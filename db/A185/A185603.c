/*
 * A185603  a(n) = floor(floor(n^(5/2))^(1/2)); complement of A185604.
 * https://oeis.org/A185603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( floor( n^(5/2) )^(1/2) )
 * Range:     u8 n <= 84, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*
 * Integer square root for 128‑bit unsigned integers.
 * Returns floor(sqrt(x)).  Implementation is the classic
 * bit‑wise algorithm that works for any 128‑bit input.
 */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126;   /* Highest power of four <= 2^128-1 */

    /* Find the highest power of four <= x */
    while (bit > x)
        bit >>= 2;

    /* Main loop */
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/*
 * Computes a(n) = floor( floor(n^(5/2))^(1/2) )
 * for n >= 1.  All arithmetic is performed exactly,
 * using 128‑bit temporaries to avoid overflow.
 */
seq_t a(idx_t n)
{
    if (n == 0)   /* index < 1 is undefined; return 0 */
        return (seq_t)0;

    unsigned __int128 n128 = (unsigned __int128)n;

    /* Compute n^5 in 128‑bit */
    unsigned __int128 n5 = n128 * n128 * n128 * n128 * n128;

    /* floor(sqrt(n^5)) */
    unsigned __int128 s = isqrt128(n5);

    /* floor(sqrt(s)) = floor( floor(n^(5/2))^(1/2) ) */
    unsigned __int128 res = isqrt128(s);

    return (seq_t)res;
}
