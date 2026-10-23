/*
 * A018458  Divisors of 441.
 * https://oeis.org/A018458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A018458: Divisors of 441 (found in ascending order) */
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divisors[9];
    static size_t divcnt = 0;
    if (!divcnt) {
        seq_t small[9], large[9];
        size_t sc = 0, lc = 0;
        /* 441 = 3² · 7², its largest divisor is 441, sqrt(441)=21 */
        for (idx_t i = 1; i * i <= (idx_t)441; ++i) {
            if ((idx_t)441 % i == 0) {
                small[sc++] = (seq_t)i;
                if (i != (idx_t)441 / i) {
                    large[lc++] = (seq_t)((idx_t)441 / i);
                }
            }
        }
        /* concatenate small (already ascending) and large (descending) reversed */
        for (size_t i = 0; i < sc; ++i) divisors[i] = small[i];
        for (size_t i = 0; i < lc; ++i) divisors[sc + i] = large[lc - 1 - i];
        divcnt = sc + lc;  /* should be 9 */
    }
    if (n == 0) return 0; /* safety for non‑positive indices */
    size_t idx = (size_t)n - 1;
    if (idx < divcnt) return divisors[idx];
    return 0; /* undefined beyond the finite sequence */
}
