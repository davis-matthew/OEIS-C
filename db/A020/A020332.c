/*
 * A020332  Numbers whose base-4 representation is the juxtaposition of two
 *          identical strings.
 * https://oeis.org/A020332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*4^floor(log_4(n)+1) + n
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base-4 concatenation of n with itself */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count base‑4 digits of n: k = ⌊log_4(n)⌋ + 1  */
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        ++k;
        tmp /= 4;
    }

    /* Compute 4^k  */
    seq_t pow = 1;
    for (idx_t i = 0; i < k; ++i)
        pow *= 4;

    /* Return n * 4^k + n  */
    return ((seq_t)n * pow) + (seq_t)n;
}
