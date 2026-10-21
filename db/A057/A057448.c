/*
 * A057448  a(n+1) = next prime such that a(n+1)-1 | (a(1)...a(n))^5.
 * https://oeis.org/A057448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A057448: a(1)=2; a(n+1)=next prime p such that (p−1)|(a(1)…a(n))^5
 *  Equivalently, all prime factors of p−1 must be among the previous terms
 *  and each with exponent ≤5.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n);   */
#include "../utils/factor_u64.h"     /* void factor_u64(uint64_t n,
                                      uint64_t *pr, uint64_t *ex, size_t *cnt);
                                      */

/* ------------------------------------------------------------------ */
/* Global memoisation state                                               */
static seq_t *seq_cache   = NULL;  /* array of sequence terms          */
static size_t seq_cap     = 0;     /* allocated capacity               */
static size_t seq_len     = 0;     /* number of terms currently stored */


/* ------------------------------------------------------------------ */
/* Test whether val is already a term of the sequence                   */
static bool seq_contains(seq_t val)
{
    for (size_t i = 0; i < seq_len; ++i) {
        if (seq_cache[i] == val) return true;
    }
    return false;
}


/* ------------------------------------------------------------------ */
/* Check candidate prime p (assumed prime)                              */
static bool candidate_valid(seq_t p)
{
    if (p < 3) return false;                /* p must be at least 3 */

    /* Factor p−1.  All factors must come from previous terms.      */
    uint64_t n      = (uint64_t)p - 1ULL;
    uint64_t *pr    = NULL;
    uint64_t *ex    = NULL;
    size_t    cnt   = 0;

    /* Allocate temporary arrays for factorisation.  Worst case is
       number of distinct prime factors, which is < 64 for 64‑bit numbers.   */
    pr = (uint64_t *)malloc(64 * sizeof(uint64_t));
    ex = (uint64_t *)malloc(64 * sizeof(uint64_t));
    if (!pr || !ex) { free(pr); free(ex); return false; }

    factor_u64(n, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] > 5ULL) { free(pr); free(ex); return false; }
        if (!seq_contains((seq_t)pr[i])) { free(pr); free(ex); return false; }
    }

    free(pr);
    free(ex);
    return true;
}


/* ------------------------------------------------------------------ */
/* Main sequence function                                                */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for indices < 1 */

    /* Initialise cache with first term 2 */
    if (seq_len == 0) {
        seq_cap   = 64;
        seq_cache = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        if (!seq_cache) return 0;
        seq_cache[0] = (seq_t)2;
        seq_len = 1;
    }

    /* Generate terms up to index n */
    while (seq_len < (size_t)n) {
        seq_t cand = seq_cache[seq_len - 1] + 1;

        /* Search for next valid prime */
        for (;;) {
            if (is_prime_big(cand) && candidate_valid(cand)) {
                /* Ensure capacity */
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(seq_cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) { /* realloc failed – return last valid term */
                        return seq_cache[seq_len - 1];
                    }
                    seq_cache = tmp;
                    seq_cap   = new_cap;
                }
                seq_cache[seq_len++] = cand;
                break; /* move to next required term */
            }
            ++cand;  /* try next integer */
        }
    }

    return seq_cache[(size_t)n - 1];
}
