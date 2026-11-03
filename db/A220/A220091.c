/*
 * A220091  Number of ways to write n=p+q+(n mod 2)q with p>q and p, q, 6q-1,
 *          6q+1 all prime.
 * https://oeis.org/A220091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (p,q) of primes such that p > q, p, q,
 *            6*q-1, 6*q+1 are all prime, and n = p + q if n is even n = p +
 *            2*q if n is odd (i.e. n = p + q + (n mod 2)*q)
 * Range:     u8 n <= 99, u16 n <= 99999, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 116 terms through n = 9431 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    bool odd = ((n & 1) != 0);

    /* upper bound for q:
       if n even: 2*q < n   => q <= (n-1)/2
       if n odd : 3*q < n   => q <= (n-1)/3
       use the safe bound for all cases: q <= n/2
    */
    idx_t qmax = n / 2;

    for (idx_t q = 2; q <= qmax; ++q)
    {
        if (!isPrimeGeneric(q))
            continue;

        seq_t qseq = q;
        seq_t sixq = qseq * 6;

        /* check twin primes 6q-1 and 6q+1 */
        if (!isPrimeGeneric(sixq - 1))
            continue;
        if (!isPrimeGeneric(sixq + 1))
            continue;

        seq_t p;
        if (odd)
        {
            /* n = p + 2q --> p = n - 2q */
            if (n <= 2 * q)   /* p would be <= 0 or <=q */
                continue;
            p = (seq_t)n - ((seq_t)(q) * 2);
        }
        else
        {
            /* n = p + q --> p = n - q */
            if (n <= q)
                continue;
            p = (seq_t)n - ((seq_t)q);
        }

        /* p must be > q and prime */
        if (p <= qseq)
            continue;
        if (!isPrimeGeneric(p))
            continue;

        ++cnt;
    }
    return cnt;
}
