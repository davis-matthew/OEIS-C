/*
 * A133624  Binomial(n+4, 4) mod n.
 * https://oeis.org/A133624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,4) mod n
 * Range:     u8 n <= 316, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd for 64‑bit values */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not part of the sequence, safe guard */

    seq_t res = 1;
    uint64_t denom = 24;               /* 4! */
    uint64_t nn = (uint64_t)n;         /* modulus value */

    for (int i = 0; i < 4; ++i)
    {
        uint64_t factor = (uint64_t)n + 4 - i;   /* n+4, n+3, n+2, n+1 */
        uint64_t g = gcd_uint64(factor, denom);
        factor /= g;
        denom /= g;

        /* multiply modulo nn */
        res = (res * (seq_t)factor) % (seq_t)nn;
    }
    /* denom should now be 1 and res equals binomial(n+4,4) mod n */
    return res % (seq_t)nn;
}
