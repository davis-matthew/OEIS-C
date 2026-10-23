/*
 * A107991  Number of spanning trees in the simple graph with nodes
 *          {1,2,...,n} and edges {i,j} if i + j > n.
 * https://oeis.org/A107991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! / floor((n+1)/2) with a(1)=1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for 64‑bit unsigned integers. */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* floor((n+1)/2) fits in uint64_t because n is uint64_t. */
    uint64_t denom = (uint64_t)(n + 1) / 2;
    seq_t prod = (seq_t)1;

    /* Build (n-1)! cancelling common factors with denom. */
    for (idx_t i = 2; i < n; ++i)
    {
        uint64_t g = gcd_uint64((uint64_t)i, denom);
        if (g > 1)          /* factor already in denom */
        {
            denom /= g;
            /* remove the same factor from the multiplier */
            prod *= (seq_t)((uint64_t)i / g);
        }
        else                /* nothing to cancel */
        {
            prod *= (seq_t)i;
        }
    }

    /* Any remaining divisor (theoretically none) is applied. */
    if (denom != 1)
        prod /= (seq_t)denom;

    return prod;
}
