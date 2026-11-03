/*
 * A074240  Numbers k such that k-th prime = sigma(sigma(k)) + 1.
 * https://oeis.org/A074240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the integers k satisfying  prime(k) = sigma(sigma(k)) + 1.
   The function returns the n-th such k (1‑based indexing). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility functions provided by the build. */
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Cache for the terms that have been found. */
static seq_t *term_cache = NULL;   /* array of found k values */
static size_t term_len   = 0;      /* number of terms currently cached */
static size_t term_cap   = 0;      /* allocated capacity of the array   */
static idx_t  searched_k = 0;      /* largest k examined so far        */

/* Append a new term to the cache, growing it if necessary. */
static void append_term(seq_t val)
{
    if (term_len == term_cap) {
        term_cap = term_cap ? term_cap * 2 : 64;
        seq_t *tmp = mem_resize(seq_t, term_cache, term_cap);
        if (!tmp) abort();  /* allocation failure → terminate */
        term_cache = tmp;
    }
    term_cache[term_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms cached. */
    while (term_len < (size_t)n) {
        /* Examine the next candidate k. */
        searched_k++;

        /* Compute sigma(k). */
        seq_t sigma1 = sigma_of((idx_t)searched_k);

        /* Compute sigma(sigma(k)).  sigma_of expects an idx_t argument, so
           we safely cast sigma1 to idx_t (the values involved are well below
           the 64‑bit limit for the range of n considered). */
        seq_t sigma2 = sigma_of((idx_t)sigma1);

        /* Candidate value for the prime. */
        seq_t candidate = sigma2 + (seq_t)1;

        /* k‑th prime.  get_prime returns a uint32_t; promote it to seq_t. */
        seq_t prime_k = (seq_t)get_prime(searched_k);

        if (candidate == prime_k) {
            append_term(searched_k);
        }
    }

    return term_cache[n - 1]; /* zero‑based indexing */
}
