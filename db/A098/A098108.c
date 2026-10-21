/*
 * A098108  a(n) = 1 if n is an odd square, otherwise 0.
 * https://oeis.org/A098108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if n is an odd square, otherwise 0
 * Range:     u8 n <= 9987, u16 n <= 9987, u32 n <= 9987, u64 n <= 9987,
 *            u128 n <= 9987, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t int_sqrt_u64(idx_t n)
{
    /* Integer sqrt using the shift-and-subtract method.  Works for 0 <= n < 2^64. */
    idx_t result = 0;
    idx_t bit = 1ULL << 62;  /* The second-to-top bit of a 64-bit number */

    if (n < 2) return n;  /* sqrt(0)=0, sqrt(1)=1 */

    /* Find the highest power of four <= n */
    while (bit > n) bit >>= 2;

    while (bit != 0) {
        if (n >= result + bit) {
            n -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }

    return result;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0)            /* even numbers cannot be odd squares */
        return (seq_t)0;

    idx_t r = int_sqrt_u64(n);   /* floor(sqrt(n)) */
    return (seq_t)((r * r == n) ? 1 : 0);
}
