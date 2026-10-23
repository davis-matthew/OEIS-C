/*
 * A194420  Numbers m such that Sum_{k=1..m} (<1/3 + k*r> - <k*r>) = 0, where
 *          r=sqrt(5) and < > denotes fractional part.
 * https://oeis.org/A194420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer m such that sum_{k=1}^{m} (
 *            {1/3 + k*sqrt(5)} - {k*sqrt(5)} ) = 0, where {x} denotes the
 *            fractional part of x
 * Range:     u8 n <= 32, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for unsigned 64‑bit numbers */
static uint64_t integer_sqrt_uint64(uint64_t n)
{
    uint64_t r = 0;
    for (int i = 63; i >= 0; --i)
    {
        uint64_t candidate = r | ((uint64_t)1 << i);
        if ((__int128)candidate * candidate <= n)
            r = candidate;
    }
    return r;
}

/* The main solver */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n==0 is unused */

    idx_t target   = n;
    idx_t found    = 0;
    idx_t k        = 0;            /* current m */
    idx_t count_low = 0;           /* number of k with frac(k*sqrt5)<=2/3 */

    while (found < target)
    {
        ++k;

        /* a_k = floor(k*sqrt(5)) = floor(sqrt(5*k^2)) */
        __int128 prod    = (__int128)5 * k * k;            /* fits in 128 bits */
        uint64_t a_k     = integer_sqrt_uint64((uint64_t)prod);

        /* test: frac(k*sqrt5) <= 2/3  <=> (a_k + 2/3)^2 >= 5*k^2 */
        __int128 lhs = (__int128)(3 * a_k + 2);
        lhs = lhs * lhs;                                   /* (3*a_k+2)^2   */
        __int128 rhs = (__int128)45 * k * k;               /* 45*k^2        */
        if (lhs >= rhs)
            ++count_low;

        /* we are only interested in multiples of 3 */
        if ((k % 3) == 0 && 3 * count_low == 2 * k)
        {
            ++found;
            if (found == target)
                return (seq_t)k;      /* Cast to seq_t (could be 32, 64 or bigger) */
        }
    }
    return 0;  /* unreachable; satisfies compiler */
}
