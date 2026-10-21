/*
 * A098962  Smallest sequence such that over all terms each prime p occurs
 *          exactly p times as prime factor; a(1)=1.
 * https://oeis.org/A098962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 8532, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098962: Smallest sequence such that every prime p occurs
   exactly p times in the prime factorisation of all terms.
   The sequence is generated greedily: starting from a(1)=1,
   each following term is the smallest integer larger than
   the previous term whose prime multiplicities do not exceed the
   remaining capacities of the primes.  The remaining capacity of a
   prime p is initially p and is decreased by the exponent of p in
   each chosen term. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/ensure_size.h"

/* ---------- Global tables ---------------------------------------- */

/* Small primes table (for factorisation).  The table is re‑built
   lazily whenever a larger limit is required. */
static uint64_t *primes = NULL;   /* array of primes */
static size_t   primes_count = 0; /* number of primes in the array */
static uint64_t primes_limit  = 0;/* largest number up to which we have sieved */

/* Remaining capacities of primes that have appeared so far.
   Each entry holds the prime and its remaining count. */
struct CapEntry {
    uint64_t prime;     /* prime value */
    uint64_t remaining; /* remaining capacity for this prime */
};
static struct CapEntry *caps    = NULL;
static size_t           cap_count  = 0; /* number of entries used */
static size_t           cap_alloc  = 0; /* allocated size of caps */

/* Sequence cache.  a(1) is stored at index 0. */
static seq_t *terms      = NULL;
static size_t terms_count = 0; /* number of terms stored */
static size_t terms_cap   = 0; /* allocated size of terms */

/* Last term generated (in uint64_t for ease of comparison). */
static uint64_t last_term = 1;

/* ---------- Helper functions ------------------------------------- */

/* Build a sieve of Eratosthenes up to "limit" (inclusive).  The
   primes are stored in the global array "primes". */
static void rebuild_primes(uint64_t limit) {
    if (limit <= primes_limit) return;          /* nothing to do */

    /* Allocate a composite flag array. */
    uint8_t *is_comp = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!is_comp) abort();

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* Count primes. */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;

    /* Allocate and fill primes array. */
    uint64_t *new_primes = (uint64_t *)realloc(primes, cnt * sizeof(uint64_t));
    if (!new_primes) abort();
    primes = new_primes;
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) primes[idx++] = i;

    primes_count = cnt;
    primes_limit = limit;

    free(is_comp);
}

/* Ensure that primes are available up to sqrt(n). */
static void ensure_primes_for_factor(uint64_t n) {
    uint64_t lim = (uint64_t)(sqrt((long double)n)) + 1;
    rebuild_primes(lim);
}

/* Append a term to the sequence cache. */
static void push_term(seq_t v) {
    ensure_size((void **)&terms, &terms_cap, terms_count + 1, sizeof(seq_t));
    terms[terms_count++] = v;
}

/* Find the index of prime p in the "caps" array; -1 if not present. */
static int find_cap_index(uint64_t p) {
    for (size_t i = 0; i < cap_count; ++i)
        if (caps[i].prime == p) return (int)i;
    return -1;
}

/* Factor n into primes and exponents.  Results are stored in
   pf[] and ex[]; *nf receives the number of distinct factors. */
static void factor_uint64(uint64_t n, uint64_t pf[20], uint64_t ex[20], size_t *nf) {
    *nf = 0;
    uint64_t rem = n;
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if (p > rem / p) break;           /* p*p > rem */
        if (rem % p == 0) {
            uint64_t cnt = 0;
            while (rem % p == 0) {
                rem /= p;
                ++cnt;
            }
            pf[(*nf)] = p;
            ex[(*nf)] = cnt;
            ++(*nf);
        }
    }
    if (rem > 1) {
        pf[(*nf)] = rem;
        ex[(*nf)] = 1;
        ++(*nf);
    }
}

/* Generate the next term of the sequence and append it to the cache. */
static void generate_next_term(void) {
    uint64_t cand = last_term + 1;
    uint64_t pf[20], ex[20];
    size_t nf;

    for (;;) {
        ensure_primes_for_factor(cand);
        factor_uint64(cand, pf, ex, &nf);

        /* First pass: check if capacities allow this candidate. */
        int ok = 1;
        for (size_t i = 0; i < nf; ++i) {
            uint64_t p = pf[i];
            uint64_t e = ex[i];
            int  idx = find_cap_index(p);
            uint64_t avail = (idx >= 0) ? caps[idx].remaining : p;
            if (avail < e) { ok = 0; break; }
        }

        if (!ok) { ++cand; continue; }

        /* Second pass: update capacities. */
        for (size_t i = 0; i < nf; ++i) {
            uint64_t p = pf[i];
            uint64_t e = ex[i];
            int idx = find_cap_index(p);
            if (idx >= 0) {
                caps[idx].remaining -= e;
            } else {
                ensure_size((void **)&caps, &cap_alloc, cap_count + 1, sizeof(struct CapEntry));
                caps[cap_count].prime = p;
                caps[cap_count].remaining = p - e;
                ++cap_count;
            }
        }

        push_term((seq_t)cand);
        last_term = cand;
        break;
    }
}

/* Initialise the sequence cache with a(1)=1. */
static void init_cache(void) {
    if (terms_count > 0) return;     /* already initialised */
    push_term((seq_t)1);            /* a(1) = 1 */
    last_term = 1;
    primes_limit = 0;               /* primes will be built lazily */
    cap_count = cap_alloc = 0;
    caps = NULL;
}

/* ---------- Public API ------------------------------------------- */

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    init_cache();
    while (terms_count <= (size_t)n) {
        generate_next_term();
    }
    return terms[n - 1];   /* terms[0] holds a(1) */
}
