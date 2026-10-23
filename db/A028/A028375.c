/*
 * A028375  Squares of (odd numbers not divisible by 5).
 * https://oeis.org/A028375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A045572(n))^2 (odd squares not divisible by 5)
 * Range:     u8 n <= 6, u16 n <= 102, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard anyway */

    /* indices: n-1 = 4*t + offset     offset = 0,1,2,3  */
    idx_t n_minus1 = n - 1;
    idx_t t      = n_minus1 / 4;
    idx_t offset = n_minus1 % 4;

    /* r values correspond to admissible remainders of k modulo 5 */
    static const idx_t r_map[4] = {0, 1, 3, 4};
    idx_t r = r_map[offset];

    /* compute the k-th admissible odd: k = 5*t + r  */
    seq_t k = (seq_t)t * (seq_t)5 + (seq_t)r;

    /* odd number = 2*k + 1  */
    seq_t odd = k * (seq_t)2 + (seq_t)1;

    /* return its square */
    return odd * odd;
}
