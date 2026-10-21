/*
 * A207893  Number of nX6 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207893: Number of n×6 binary arrays avoiding the horizontal patterns
   000 and 101 and the vertical patterns 000 and 111.
   Computed by dynamic programming over admissible 6‑bit rows. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* external integer types supplied by the build system: */
seq_t a(idx_t n)
{
    /*--------------------------------------------------------------------
      Step 1: enumerate the 6‑bit rows that contain no 000 or 101 on
      any three consecutive columns.  There are 19 such rows.
      The result is cached in static arrays that survive between calls. */
    static bool init = false;
    static uint8_t rows[19];
    static size_t rcnt;
    static uint32_t trans[361];            /* rcnt × rcnt transitions */

    if (!init) {
        size_t cnt = 0;
        for (int mask = 0; mask < 64; ++mask) {
            bool ok = true;
            for (int s = 0; s <= 3; ++s) {
                int bits = (mask >> s) & 7;
                if (bits == 0 || bits == 0b101) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                rows[cnt] = (uint8_t)mask;
                ++cnt;
            }
        }
        rcnt = cnt;          /* should be 19 */

        /*----------------------------------------------------------------
          Build transition bitmasks: trans[a*rcnt+b] contains a bitmask of
          all rows c such that rows a,b,c together avoid vertical 000 and 111.
          ---------------------------------------------------------------*/
        for (size_t a = 0; a < rcnt; ++a) {
            for (size_t b = 0; b < rcnt; ++b) {
                uint32_t m = 0;
                for (size_t c = 0; c < rcnt; ++c) {
                    bool ok = true;
                    for (int col = 0; col < 6; ++col) {
                        int ba = (rows[a] >> col) & 1;
                        int bb = (rows[b] >> col) & 1;
                        int bc = (rows[c] >> col) & 1;
                        if ((ba == 0 && bb == 0 && bc == 0) ||
                            (ba == 1 && bb == 1 && bc == 1)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) m |= (1u << c);
                }
                trans[a * rcnt + b] = m;
            }
        }
        init = true;
    }

    /*--------------------------------------------------------------------
      Step 2: handle trivial cases n==1, n==2.
      For n==1 the rows are independent; for n==2 vertical patterns
      are impossible, so the count is rcnt^2. */
    if (n == 0) return (seq_t)0;           /* not used (offset 1) */
    if (n == 1) return (seq_t)rcnt;        /* 19 */
    if (n == 2) {
        seq_t v = (seq_t)rcnt;
        return v * v;                      /* 361 */
    }

    /*--------------------------------------------------------------------
      Step 3: dynamic programming for n >= 3.  dp[i] holds the number of
      ways to build the grid ending with rows (r1,r2), where i = r1*rcnt+r2.
      At each new step we choose a new row r3 that is compatible with
      (r1,r2). */
    static seq_t dp[361];
    static seq_t dp_next[361];

    /* initialise dp for the first two rows: all pairs are admissible */
    for (size_t i = 0; i < rcnt * rcnt; ++i) dp[i] = (seq_t)1;

    for (idx_t step = 3; step <= n; ++step) {
        /* clear next array */
        for (size_t i = 0; i < rcnt * rcnt; ++i) dp_next[i] = 0;

        for (size_t a = 0; a < rcnt; ++a) {
            for (size_t b = 0; b < rcnt; ++b) {
                size_t idx = a * rcnt + b;
                seq_t cur = dp[idx];
                if (cur == 0) continue;
                uint32_t mask = trans[idx];
                for (size_t c = 0; c < rcnt; ++c) {
                    if (mask & (1u << c)) {
                        size_t next_idx = b * rcnt + c;
                        dp_next[next_idx] += cur;
                    }
                }
            }
        }
        /* swap dp and dp_next */
        for (size_t i = 0; i < rcnt * rcnt; ++i) dp[i] = dp_next[i];
    }

    /*--------------------------------------------------------------------
      Final step: sum all counts for the last pair of rows. */
    seq_t result = 0;
    for (size_t i = 0; i < rcnt * rcnt; ++i) result += dp[i];
    return result;
}
