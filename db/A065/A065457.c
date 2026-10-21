/*
 * A065457  Period of the flip-riffle shuffle function on a deck of 2n cards.
 * https://oeis.org/A065457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n = 1, otherwise a(n) = 2·ord_{2n-1}(2) if
 *            ord_{2n-1}(2) is odd, else a(n) = ord_{2n-1}(2). Where ord_m(x)
 *            is the multiplicative order of x modulo m
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == 1)
        return (seq_t)2;

    /* m = 2n - 1 (always odd for n > 1) */
    idx_t m = 2 * n - 1;
    uint64_t m64 = (uint64_t)m;

    /* Find multiplicative order of 2 modulo m */
    uint64_t val = 2 % m64;          /* current power of 2 modulo m */
    seq_t   ord = 1;                 /* order r starts at 1 */

    /* Iterate until we find the smallest r with 2^r ≡ 1 (mod m) */
    while (val != 1) {
        val = (val * 2) % m64;       /* multiply by 2 modulo m */
        ord++;
    }

    /* Adjust period: if ord is odd, we need 2*ord, else ord itself */
    if ((ord & (seq_t)1) == (seq_t)1)
        ord <<= 1;                    /* multiply by 2 using bit shift */

    return ord;
}
