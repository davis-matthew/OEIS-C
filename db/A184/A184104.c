/*
 * A184104  n+floor(5*sqrt(n)); complement of A184105.
 * https://oeis.org/A184104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+floor(5*sqrt(n))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n + ⌊5√n⌋ for n ≥ 1 using only integer arithmetic. */
#include <stdint.h>

static seq_t sqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64);
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset is 1, values for n < 1 are not needed */

    unsigned __int128 product = (unsigned __int128)n * 25;   /* compute 25*n safely in 128‑bit */
    seq_t sqrtv = sqrt_uint128(product);                    /* ⌊√(25*n)⌋ = ⌊5√n⌋   */
    return (seq_t)n + sqrtv;                               /* a(n) = n + ⌊5√n⌋  */
}
