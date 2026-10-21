/*
 * A080801  Similar to A080799 but count only addition steps.
 * https://oeis.org/A080801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A023416(n+1)+1, * where A023416(m) = number of zeros in
 *            the binary representation of * m - 2^⌊log₂m⌋, written with
 *            ⌊log₂m⌋ bits
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* p = n+1 (index for A023416) */
    idx_t p = n + 1;

    /* find k = floor(log2(p)) and the corresponding power of two t = 2^k */
    idx_t t = 1;
    idx_t k = 0;
    while ((t << 1) <= p) {
        t <<= 1;
        ++k;
    }

    /* m = p - 2^k */
    idx_t m = p - t;

    /* count set bits in m (popcount) */
    idx_t pc = 0;
    idx_t x = m;
    while (x) {
        pc += 1;
        x &= x - 1;        /* clear least significant set bit */
    }

    /* number of zeros in k-bit representation of m */
    idx_t zeros = k - pc;

    /* a(n) = zeros + 1 */
    return (seq_t)(zeros + 1);
}
