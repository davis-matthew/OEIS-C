/*
 * A095686  Half the number of divisors of nonsquares (A000037).
 * https://oeis.org/A095686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A000037(n))/2 A000037(n) = the nth non‑square.
 *            For a non‑square τ(n) is even, so a(n) = τ(n)/2
 * Range:     u8 n <= 412, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation is independent of the utilities
   package and works for any underlying 8, 16, 32, 64 or arbitrary‑precision
   integer type defined by the build system.  It uses a simple
   linear search to find the nth non‑square and a trial‑division
   routine to count its divisors.  For n up to about 10 000 all
   operations complete well within the time limit. */
#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit values.  */
static inline uint64_t floor_sqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    for (int i = 31; i >= 0; --i)               /* 2^32 > 2^64, so 32 bits suffice */
    {
        uint64_t nxt = r | ((uint64_t)1 << i);
        if ( (__uint128_t)nxt * nxt <= x )
            r = nxt;
    }
    return r;
}

/* Count the number of positive divisors of n (τ(n)).  */
static seq_t divisor_count(seq_t n)
{
    seq_t result = 1;
    seq_t temp   = n;

    /* 2 is a special case to keep the loop starting at 3 */
    if ((temp & 1) == 0)
    {
        seq_t exp = 0;
        while ((temp & 1) == 0)
        {
            temp >>= 1;
            ++exp;
        }
        result *= (exp + 1);
    }

    for (seq_t p = 3; p <= temp / p; p += 2)
    {
        if (temp % p == 0)
        {
            seq_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }

    if (temp > 1)                /* remaining prime factor */
        result *= 2;

    return result;
}

/* Find the nth non‑square (1‑based) and return τ(n)/2.  */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1, but guard against misuse */
        return 0;

    idx_t count = 0;
    uint64_t candidate = 1;      /* start from 1, which is a square */

    while (count < n)
    {
        ++candidate;
        uint64_t sqrt = floor_sqrt_u64(candidate);
        if (sqrt * sqrt != candidate)   /* not a perfect square */
            ++count;
    }

    seq_t nonsquare = (seq_t)candidate;
    seq_t tau = divisor_count(nonsquare);
    return tau / 2;          /* unordered divisor pairs */
}
