/*
 * A097508  a(n) = floor(n*(sqrt(2)-1)).
 * https://oeis.org/A097508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n * (sqrt(2)-1)) = floor(sqrt(2*n*n)) - n
 * Range:     u8 n <= 618, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128.
   Returns floor(sqrt(x)).  The result always fits in 64 bits. */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64);  /* high > sqrt(x) */
    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Returns a(n) for A097508. */
seq_t a(idx_t n)
{
    /* Compute floor(sqrt(2*n*n)) - n.
       Using 128‐bit intermediate to avoid overflow. */
    unsigned __int128 prod = (unsigned __int128)n * n;
    prod *= 2;                        /* 2*n*n  */
    unsigned __int128 s = isqrt128(prod);   /* floor(sqrt(2*n*n)) */
    return (seq_t)(s - (unsigned __int128)n);
}
