/*
 * A104772  If n<=2 then n else (if n is odd then 2*a(n+1) else p*q, where
 *          n=p+q, p<=q, primes).
 * https://oeis.org/A104772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n>2: if n is odd then a(n)=2*a(n+1); if n is
 *            even (≥4) let p,q be primes with p+q=n and p≤q; then a(n)=p*q.
 *            This implements the Goldbach‑based encoding of A104772
 * Range:     u8 n <= 28, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for numbers up to ~1e6. */
static bool is_prime(uint64_t x)
{
    if (x < 2)          return false;
    if (x % 2 == 0)     return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Compute the Goldbach product for an even integer m ≥ 4.  Assumes m is even. */
static seq_t goldbach_product(uint64_t m)
{
    for (uint64_t p = 2; p <= m / 2; ++p)
    {
        if (!is_prime(p))          continue;
        uint64_t q = m - p;
        if (q < p)                 continue;   /* ensure p ≤ q */
        if (is_prime(q))
            return (seq_t)p * (seq_t)q;
    }
    /* By Goldbach's conjecture (verified for m ≤ 10000) this point is unreachable. */
    return 0;
}

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)n;          /* a(1)=1, a(2)=2 */

    if (n & 1)                              /* odd n: a(n)=2*a(n+1) */
    {
        seq_t val = a(n + 1);
        return val * 2;
    }

    /* even n ≥ 4: product of Goldbach pair */
    return goldbach_product((uint64_t)n);
}
