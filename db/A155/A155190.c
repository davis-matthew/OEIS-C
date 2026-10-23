/*
 * A155190  Non-triangular numbers having a unique representation as p + t,
 *          where p is prime and t is a triangular number.
 * https://oeis.org/A155190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   triangulars
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155190: Non‑triangular numbers having a unique representation
   as p + t, where p is prime and t is triangular.  The function builds the
   required terms on demand and returns the nth term (n ≥ 1). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/triangulars.h"

/* External function required by the test harness. */
seq_t a(idx_t n);

/* ---------- Internal static state ------------------------------------- */
static seq_t *terms = NULL;          /* array of computed terms          */
static size_t terms_cnt = 0;          /* number of stored terms           */
static idx_t   high = 0;             /* current maximal number examined  */

/* ---------- Helper: generate primes up to limit (inclusive) ------------
   Returns a dynamically allocated array of primes and stores the
   number of primes in *cnt.  The caller must free the array. */
static uint64_t *sieve_primes(idx_t limit, size_t *cnt)
{
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return NULL;

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t k = p * p; k <= limit; k += p)
                is_prime[k] = false;

    /* Count primes */
    *cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++(*cnt);

    /* Allocate array of primes */
    uint64_t *primes = (uint64_t*)malloc((*cnt) * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return NULL;
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = (uint64_t)i;

    free(is_prime);
    return primes;
}

/* ---------- Core: build terms up to covering 'need' terms ---------------
   This function may be called repeatedly to extend the stored list.   */
static void build_terms(idx_t need)
{
    if (terms_cnt >= need) return;          /* already have enough */

    /* Start with an upper bound that will definitely contain all required
       terms.  We double the bound until it works. */
    idx_t UB = (high > 0) ? high : 200000ULL;  /* start with 200k */

    while (1) {
        /* Re-sieve up to UB */
        size_t prime_cnt = 0;
        uint64_t *primes = sieve_primes(UB, &prime_cnt);
        if (!primes) break;  /* out of memory, give up */

        /* Triangular numbers up to UB */
        size_t tri_cnt = 0;
        uint64_t *tris = triangulars(UB, &tri_cnt);
        if (!tris) { free(primes); break; }

        /* Count representations p + t for each n <= UB */
        uint16_t *cnt = (uint16_t*)calloc((UB + 1), sizeof(uint16_t));
        if (!cnt) { free(primes); free(tris); break; }

        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            uint64_t p = primes[pi];
            for (size_t ti = 0; ti < tri_cnt; ++ti) {
                uint64_t t = tris[ti];
                uint64_t sum = p + t;
                if (sum > (uint64_t)UB) break;
                ++cnt[sum];
            }
        }

        /* Mark triangular numbers for quick lookup */
        bool *is_tri = (bool*)calloc((UB + 1), sizeof(bool));
        if (!is_tri) { free(primes); free(tris); free(cnt); break; }
        for (size_t ti = 0; ti < tri_cnt; ++ti)
            if (tris[ti] <= (uint64_t)UB)
                is_tri[tris[ti]] = true;

        /* Gather sequence terms up to UB */
        size_t new_cap = 100000;
        seq_t *new_terms = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_terms) { free(primes); free(tris); free(cnt); free(is_tri); break; }
        size_t new_cnt = 0;
        for (idx_t x = 2; x <= UB; ++x) {
            if (!is_tri[x] && cnt[x] == 1) {
                if (new_cnt >= new_cap) {
                    new_cap *= 2;
                    seq_t *tmp = (seq_t*)realloc(new_terms, new_cap * sizeof(seq_t));
                    if (!tmp) break; /* allocation failure */
                    new_terms = tmp;
                }
                new_terms[new_cnt++] = (seq_t)x;
            }
        }

        /* Clean up temporaries */
        free(is_tri);
        free(cnt);
        free(primes);
        free(tris);

        if (new_cnt == 0) {
            /* No new terms found; increase UB and try again */
            UB *= 2;
            free(new_terms);
            continue;
        }

        /* Replace stored terms if we found more */
        if (new_cnt > terms_cnt) {
            free(terms);
            terms = new_terms;
            terms_cnt = new_cnt;
            high = UB;
        } else {
            free(new_terms);
        }

        /* If we now have enough terms, stop */
        if (terms_cnt >= need) return;

        /* Otherwise, increase UB and try again */
        UB *= 2;
    }
}

/* ---------- Public function implementation ----------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* Undefined for n < 1 */

    if (terms_cnt < (size_t)n)
        build_terms(n);

    if (n <= terms_cnt)
        return terms[n - 1];
    else
        return (seq_t)0;           /* Should not happen if build_terms succeeded */
}
