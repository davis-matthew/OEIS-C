/*
 * A047824  Becomes prime after exactly 5 iterations of f(x) = sum of prime
 *          factors of x.
 * https://oeis.org/A047824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Generates the n-th term of OEIS A047824 by scanning
   positive integers and testing the 5‑iteration property of
   f(x) = sum of distinct prime factors. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <limits.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Upper bound for the sieve used for prime factorisation. */
#define SPF_LIMIT 2000000ULL

/* Static state: cache of found terms, current scanning position,
   and sieved smallest prime factors. */
static seq_t *term_cache = NULL;
static size_t term_cache_len = 0;
static size_t term_cache_cap = 0;

static uint32_t *spf = NULL;      /* smallest prime factor array */
static size_t spf_size = 0;

/* Initialise the SPF array once. */
static void init_spf(void) {
    if (spf != NULL) return;
    spf_size = (size_t)SPF_LIMIT + 1;
    spf = (uint32_t *)calloc(spf_size, sizeof(uint32_t));
    if (!spf) return;  /* allocation failure, let caller handle */

    for (size_t i = 2; i < spf_size; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i < spf_size) {
                for (size_t j = i * i; j < spf_size; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
}

/* Compute sum of distinct prime factors (no multiplicity) using SPF. */
static seq_t sum_distinct_prime_factors(uint64_t n) {
    seq_t sum = 0;
    while (n > 1) {
        uint32_t p = spf[(size_t)n];
        if (p == 0) p = (uint32_t)n;  /* n is prime larger than SPF_LIMIT */
        sum += p;
        while (n % p == 0) n /= p;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, but avoid UB */

    /* Initialise static resources on first call. */
    static bool first = true;
    if (first) {
        first = false;
        init_spf();
    }

    /* Find terms until we have enough. */
    static idx_t current = 2;          /* scanning starts at 2 */
    for (;;) {
        if (term_cache_len >= (size_t)n) break;

        seq_t cand = (seq_t)current;
        bool ok = true;
        uint64_t x = (uint64_t)current;

        for (int i = 1; i <= 5; ++i) {
            seq_t y = sum_distinct_prime_factors(x);
            bool prime = isPrimeGeneric(y);
            if (prime) {
                if (i == 5) { /* reached prime exactly at 5th iteration */
                    ok = true;
                    break;
                } else {      /* prime too early */
                    ok = false;
                    break;
                }
            } else {
                if (i == 5) { /* not prime after 5 steps */
                    ok = false;
                    break;
                }
            }
            x = (uint64_t)y;  /* next iteration */
        }

        if (ok) {
            if (term_cache_len + 1 > term_cache_cap) {
                grow_seq(&term_cache, &term_cache_cap, term_cache_len + 1);
            }
            term_cache[term_cache_len++] = cand;
        }
        ++current;
    }

    return term_cache[n - 1];
}
