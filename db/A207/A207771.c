/*
 * A207771  Number of nX5 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A207771
   Number of n×5 binary arrays avoiding
   horizontally 000 and 111, and vertically 001 and 010.
   Computation by dynamic programming over row pairs. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static data built on first call. */
    /* Number of possible rows (5 bits) = 32. */
    /* State index = a*32 + b for pair of rows (a,b). */
    static bool ready = false;
    static bool allowed[32];
    static unsigned char transCnt[1024];
    static unsigned short trans[1024][32];  /* maximum 32 transitions per state */

    if (!ready) {
        /* admissible rows (no horizontal 000 or 111) */
        for (int r = 0; r < 32; ++r) {
            bool ok = true;
            for (int pos = 0; pos < 3 && ok; ++pos) {
                int pat = (r >> pos) & 7;  /* 3 consecutive bits */
                if (pat == 0 || pat == 7)  /* 000 or 111 */
                    ok = false;
            }
            allowed[r] = ok;
        }

        /* build transition table */
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                unsigned idx = a * 32 + b;
                if (!allowed[a] || !allowed[b]) {
                    transCnt[idx] = 0;
                    continue;
                }
                unsigned cnt = 0;
                for (int c = 0; c < 32; ++c) {
                    if (!allowed[c]) continue;
                    bool ok = true;
                    for (int col = 0; col < 5; ++col) {
                        int pa = (a >> col) & 1;
                        int pb = (b >> col) & 1;
                        int pc = (c >> col) & 1;
                        if ((pa == 0 && pb == 0 && pc == 1) ||
                            (pa == 0 && pb == 1 && pc == 0)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        trans[idx][cnt++] = b * 32 + c;
                    }
                }
                transCnt[idx] = cnt;
            }
        }
        ready = true;
    }

    /* Handle trivial cases */
    if (n <= 1) return (seq_t)(n == 1 ? 16 : 0);

    /* DP arrays */
    seq_t dp_cur[1024];
    seq_t dp_next[1024];
    for (unsigned i = 0; i < 1024; ++i)
        dp_cur[i] = 0;

    /* initialize for the first two rows (n = 2) */
    for (int a = 0; a < 32; ++a)
        if (allowed[a])
            for (int b = 0; b < 32; ++b)
                if (allowed[b])
                    dp_cur[a * 32 + b] = (seq_t)1;

    /* steps to add beyond two rows */
    idx_t steps = n - 2;
    for (idx_t step = 0; step < steps; ++step) {
        for (unsigned i = 0; i < 1024; ++i)
            dp_next[i] = 0;
        for (unsigned s = 0; s < 1024; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            unsigned cnt = transCnt[s];
            for (unsigned k = 0; k < cnt; ++k) {
                unsigned nxt = trans[s][k];
                dp_next[nxt] += val;
            }
        }
        for (unsigned i = 0; i < 1024; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* sum over all ending states */
    seq_t total = 0;
    for (unsigned i = 0; i < 1024; ++i)
        total += dp_cur[i];
    return total;
}
