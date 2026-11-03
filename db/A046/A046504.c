/*
 * A046504  Primes with multiplicative persistence value 4.
 * https://oeis.org/A046504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 750, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 8756 (2026-11-03)
 * Helpers:   get_digits, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose multiplicative persistence is 4  
   Computed by enumerating primes and testing the persistence. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include <limits.h>
#include "../utils/get_digits.h"
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return the multiplicative persistence of v */
static int multiplicative_persistence(seq_t v)
{
    int steps = 0;
    const seq_t ten = (seq_t)10;
    while (v >= ten) {
        uint8_t buf[64];
        size_t len = 0;
        get_digits(v, buf, &len);
        seq_t prod = 1;
        for (size_t i = 0; i < len; ++i)
            prod *= buf[i];
        v = prod;
        ++steps;
    }
    return steps;
}

seq_t a(idx_t n)
{
    static seq_t *primes   = NULL;
    static size_t  primes_cap = 0;
    static size_t  primes_cnt = 0;
    static seq_t   last_candidate = 0;
    static bool    initialized = false;

    if (!initialized) {
        primes   = NULL;
        primes_cap = 0;
        primes_cnt = 0;
        last_candidate = 0;
        initialized = true;
    }

    while (primes_cnt < n) {
        seq_t cand;
        if (last_candidate == 0)           /* first prime */
            cand = 2;
        else {
            cand = last_candidate + 1;
            if (cand != (seq_t)2 && (cand % (seq_t)2 == 0))
                cand += 1;                /* skip even numbers after 2 */
        }

        /* find next prime */
        while (!isPrimeGeneric(cand))
            cand += 2;

        last_candidate = cand;

        if (multiplicative_persistence(cand) == 4) {
            if (primes_cnt >= primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 128;
                primes = seq_realloc(primes, primes_cap, new_cap);
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = cand;
        }
    }

    return primes[n - 1];
}
