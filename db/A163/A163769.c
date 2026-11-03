/*
 * A163769  Primes p such that 2*p+3 is not prime.
 * https://oeis.org/A163769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 969 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate successive primes p and keep those for which
   2*p+3 is composite.  The results are memoised so that calls to a(n)
   can arrive in any order. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

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

/* static cache for memoised terms */
static seq_t *cache      = NULL; /* array holding computed terms          */
static size_t cache_len  = 0;     /* number of terms stored                */
static size_t cache_cap  = 0;     /* allocated capacity                    */

/* next prime that has not yet been examined */
static uint64_t next_candidate = 2;

/* ------------------------------------------------------------------ */
/*  a(idx_t n) – return the n‑th term of the sequence                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* generate terms until we have at least n entries */
    while (cache_len < n)
    {
        /* fetch the next prime */
        uint64_t p = next_prime(next_candidate);
        next_candidate = p + 1u;  /* advance for the next search */

        /* test whether 2*p+3 is composite */
        seq_t val = (seq_t)p;
        seq_t two_p_plus_three = val * 2 + 3;  /* seq_t arithmetic */

        if (!isPrimeGeneric(two_p_plus_three))
        {
            /* grow cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *newbuf = mem_resize(seq_t, cache, new_cap);
                if (!newbuf) abort(); /* memory exhausted */
                cache = newbuf;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    /* return the requested term (offset 1) */
    return cache[n - 1];
}
