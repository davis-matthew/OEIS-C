/*
 * A164884  a(n) = image of n under the base-2 Kaprekar map n -> (n with
 *          digits sorted into descending order) - (n with digits sorted into
 *          ascending order).
 * https://oeis.org/A164884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (2^{popcount(n)} - 1) * (2^{len(n)-popcount(n)} - 1)
 *            where len(n) = floor(log2(n)) + 1 (len(0)=1)
 * Range:     u8 n <= 256, u16 n <= 9986, u32 n <= 9986, u64 n <= 9986,
 *            u128 n <= 9986, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* compute the base‑2 Kaprekar map for index n */
seq_t a(idx_t n)
{
    /* special case: n = 0 returns 0 */
    if (n == 0) return 0;

    /* find len(n) : number of bits needed to represent n */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* count 1‑bits in n */
    idx_t ones = 0;
    tmp = n;
    while (tmp) {
        ones += tmp & 1;
        tmp >>= 1;
    }

    /* zeros = len - ones */
    idx_t zeros = len - ones;

    /* if all bits are 0 or all bits are 1, the difference is 0 */
    if (ones == 0 || zeros == 0) return 0;

    /* build (2^ones - 1) and (2^zeros - 1) using only shifts of 1 bit
       (prevents shifts >= width of seq_t, which would be undefined. */
    seq_t B = 0;
    seq_t one = 1;
    for (idx_t i = 0; i < ones; ++i)
        B = (B << 1) | one;

    seq_t F = 0;
    for (idx_t i = 0; i < zeros; ++i)
        F = (F << 1) | one;

    return B * F;
}
