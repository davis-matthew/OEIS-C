/*
 * A177054  Numbers k such that each of the decimal digits is a quadratic
 *          residue modulo k.
 * https://oeis.org/A177054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Determination of A177054: numbers k such that every decimal digit of k is a quadratic residue modulo k.
 * a(n) = the nth such integer (offset 1). */
/* The algorithm tests integers sequentially, establishing the
 * result by checking each decimal digit against k's prime powers
 * using the Legendre/Jacobi symbol. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/jacobi_symbol.h"

/* Test whether the single decimal digit d is a quadratic residue modulo k.
 * `primes` and `exps` contain the prime factorisation of k. */
static bool digit_is_quadratic_residue(uint64_t k, uint64_t d,
                                       const uint64_t primes[64], const uint8_t exps[64], size_t pc)
{
    for (size_t i = 0; i < pc; ++i) {
        uint64_t p = primes[i];
        uint8_t  e = exps[i];

        if (p == 2ULL) {                         /* special handling for powers of 2 */
            if (e == 1ULL)                       /* mod 2: every digit ≡ 0 or 1 */
                continue;
            if (e == 2ULL) {                     /* mod 4: residues 0 or 1 */
                if (d % 4ULL != 0ULL && d % 4ULL != 1ULL)
                    return false;
                continue;
            }                                   /* e >= 3: residues 0,1,4 mod 8 */
            if (d % 4ULL != 0ULL && d % 8ULL != 1ULL)
                return false;
            continue;
        }

        /* odd prime factor */
        uint64_t temp = d;
        uint8_t  f = 0;                 /* exponent of p dividing d */
        while (f < e && temp % p == 0ULL) {
            temp /= p;
            ++f;
        }
        if (f >= e)                      /* d ≡ 0 (mod p^e) */
            continue;

        /* must be even exponent of p, and Legendre symbol for u = d/p^f */
        if (f % 2 == 1)
            return false;
        int ls = jacobi_symbol(temp, p);
        if (ls != 1)
            return false;
    }
    return true;
}

/* Check if all decimal digits of k are quadratic residues modulo k. */
static bool is_valid_k(uint64_t k)
{
    /* factorisation of k into primes^exps */
    uint64_t primes[64];
    uint8_t  exps [64];
    size_t   pc = 0;
    uint64_t tmp = k;

    for (uint64_t p = 2ULL; p * p <= tmp; ++p) {
        if (tmp % p == 0ULL) {
            uint8_t e = 0;
            while (tmp % p == 0ULL) {
                tmp /= p;
                ++e;
            }
            primes[pc] = p;
            exps [pc] = e;
            ++pc;
        }
    }
    if (tmp > 1ULL) {                        /* remaining prime factor */
        primes[pc] = tmp;
        exps [pc] = 1;
        ++pc;
    }

    /* test each decimal digit */
    uint64_t t = k;
    while (t > 0ULL) {
        uint64_t d = t % 10ULL;
        t /= 10ULL;
        if (!digit_is_quadratic_residue(k, d, primes, exps, pc))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* undefined for n=0, but avoid UB */

    static seq_t *cache = NULL;   /* 0‑based cache of found terms */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_k = 0;

    /* Enough terms already cached? */
    if (cache_len >= n)
        return cache[n - 1];

    /* Grow cache if needed */
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Search for new terms */
    for (uint64_t k = last_k + 1; cache_len < n; ++k) {
        if (is_valid_k(k)) {
            cache[cache_len++] = (seq_t)k;
        }
    }

    /* Keep track of the last inspected integer */
    last_k = (cache_len > 0) ? (uint64_t)cache[cache_len - 1] : 0;

    return cache[n - 1];
}
