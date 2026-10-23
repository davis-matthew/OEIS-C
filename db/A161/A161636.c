/*
 * A161636  Number of reduced words of length n in the Weyl group A_31.
 * https://oeis.org/A161636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{31} (1 + x + … + x^k).
 *            Equivalently, the number of ways to choose integers a_1,…,a_31
 *            with 0 ≤ a_k ≤ k and ∑ a_k = n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t table[497];   /* 31*32/2 = 496 is the maximum non‑zero length */
    static bool ready = false;

    if (!ready) {
        /* initialise */
        for (idx_t i = 0; i <= 496; ++i)
            table[i] = 0;
        table[0] = 1;          /* empty product */

        idx_t curmax = 0;      /* current maximal degree processed */

        for (idx_t k = 1; k <= 31; ++k) {
            seq_t next[497];
            for (idx_t i = 0; i <= 496; ++i)
                next[i] = 0;

            /* convolve current polynomial with (1 + x + … + x^k) */
            for (idx_t i = 0; i <= curmax; ++i) {
                seq_t val = table[i];
                if (val == 0) continue;
                for (idx_t j = 0; j <= k && i + j <= 496; ++j)
                    next[i + j] += val;
            }

            for (idx_t i = 0; i <= 496; ++i)
                table[i] = next[i];

            curmax = (curmax + k > 496) ? 496 : curmax + k;
        }

        ready = true;
    }

    if (n <= 496)
        return table[n];
    else
        return 0;
}
