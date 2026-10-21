/*
 * A207091  Number of 5Xn 0..1 arrays avoiding 0 0 0 horizontally and 0 0 1
 *          vertically.
 * https://oeis.org/A207091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays with no horizontal 000 and no
 *            vertical 001
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming over columns.  Uses a state consisting of, for each of the 5
   rows, the bits of the two previous columns (older, younger).  The transition table is built
   once per program.  For n <= 0 the function returns 0. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* ------------------------------------------------------------------ */
    /* 1. Enumerate all column patterns that avoid a vertical 0 0 1.
       There are 2^5 possible bit patterns. */
    static int patterns[32];
    static int vcnt = 0;
    static bool pat_init = false;

    if (!pat_init) {
        for (int p = 0; p < 32; ++p) {
            bool ok = true;
            for (int i = 0; i < 3 && ok; ++i) {   /* check rows i,i+1,i+2 */
                int b0 = (p >> i)     & 1;
                int b1 = (p >> (i+1)) & 1;
                int b2 = (p >> (i+2)) & 1;
                if (b0 == 0 && b1 == 0 && b2 == 1)
                    ok = false;
            }
            if (ok) {
                patterns[vcnt++] = p;
            }
        }
        pat_init = true;
    }

    /* ------------------------------------------------------------------ */
    /* 2. Pre‑compute transition table.
       State is 10 bits: for each of the 5 rows two bits (older, younger). */
    static int16_t trans[1 << 10][32];
    static bool trn_init = false;

    if (!trn_init) {
        for (int s = 0; s < (1 << 10); ++s) {
            for (int k = 0; k < vcnt; ++k) {
                int p   = patterns[k];
                int ns  = 0;
                bool bad = false;
                for (int r = 0; r < 5; ++r) {
                    int bits   = (s >> (2 * r)) & 3;   /* older <<1 | younger */
                    int older  = (bits >> 1) & 1;
                    int younger= bits & 1;
                    int nb     = (p >> r) & 1;         /* new bit for this row */

                    /* horizontal 000 check */
                    if (older == 0 && younger == 0 && nb == 0) {
                        bad = true;
                        break;
                    }

                    int nxt = (younger << 1) | nb;       /* new pair */
                    ns |= nxt << (2 * r);
                }
                trans[s][k] = bad ? -1 : (int16_t)ns;
            }
        }
        trn_init = true;
    }

    /* ------------------------------------------------------------------ */
    /* 3. Dynamic programming over the n columns. */
    static seq_t dp_cur[1 << 10];
    static seq_t dp_next[1 << 10];

    /* initialise: two fictitious columns of all 1s to avoid detecting
       a 000 at the beginning of the real array. */
    for (int i = 0; i < (1 << 10); ++i)
        dp_cur[i] = 0;
    dp_cur[(1 << 10) - 1] = 1;  /* state 1111111111 */

    for (idx_t col = 0; col < n; ++col) {
        for (int i = 0; i < (1 << 10); ++i)
            dp_next[i] = 0;

        for (int s = 0; s < (1 << 10); ++s) {
            seq_t cur = dp_cur[s];
            if (cur == 0) continue;
            for (int k = 0; k < vcnt; ++k) {
                int ns = trans[s][k];
                if (ns < 0) continue;
                dp_next[ns] += cur;
            }
        }

        /* swap current with next */
        for (int i = 0; i < (1 << 10); ++i)
            dp_cur[i] = dp_next[i];
    }

    /* Sum over all possible terminal states. */
    seq_t result = 0;
    for (int i = 0; i < (1 << 10); ++i)
        result += dp_cur[i];

    return result;
}
