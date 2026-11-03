/*
 * A141465  Prime transformed products of prime factors of the composites, the
 *          largest prime decremented by 2, the smallest by 1.
 * https://oeis.org/A141465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   factor_u64, is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141465: a(n) is the nth prime obtained by transforming the
   prime factors of the n-th composite number according to the
   definition in the OEIS entry. The algorithm scans successive
   composite integers, computes their smallest and largest prime
   factors, applies the transformation
       k * (pmax-2) * (pmin-1) / (pmin * pmax),
   and returns the nth prime value produced. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n=0, but return 0 safely */

    idx_t  count = 0;                  /* number of valid primes found */
    uint64_t k = 4;                    /* start from the first composite */

    /* buffers for factor_u64: 64 prime factors + exponents suffice for 64‑bit numbers */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t   num_factors;

    for (;; ++k)
    {
        if (is_prime(k)) continue;     /* skip primes, we only need composites */

        /* factorization to obtain smallest and largest prime factors */
        factor_u64(k, primes, exponents, &num_factors);
        uint64_t pmin = primes[0];
        uint64_t pmax = primes[num_factors - 1];

        uint64_t denom = pmin * pmax;
        if (denom == 0) continue;      /* safety, should never happen */

        /* compute product = k / (pmin*pmax) * (pmax-2)*(pmin-1) */
        seq_t t = (seq_t)(k / denom);
        seq_t prod = t;
        prod *= (seq_t)(pmax - 2);
        prod *= (seq_t)(pmin - 1);

        /* if the result is a prime, record it */
        if (prod >= 2 && is_prime_big(prod))
        {
            ++count;
            if (count == n) return prod;
        }
    }
}
