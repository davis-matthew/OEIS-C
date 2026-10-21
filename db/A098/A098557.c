/*
 * A098557  Expansion of e.g.f. (1/2)*(1+x)*log((1+x)/(1-x)).
 * https://oeis.org/A098557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (derived from the e.g.f. definition):
 *      a(0) = 0
 *      a(1) = 1
 *      a(n) = (n-1)!  +  (n even ? (n-2)! : 0)   for n ≥ 2
 *
 *  This follows from
 *      a(n+1) = n! + (n-1)!·(1-(-1)^n)/2
 *  which after reindexing gives the closed form above.
 *
 *  The implementation caches factorials and the sequence itself
 *  to allow calls with arbitrary ordering of indices.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- factorial cache ------------------------------------ */
static seq_t *fact_cache = NULL;      /* fact_cache[i] = i! */
static size_t fact_cap   = 0;         /* current allocation size */
static size_t fact_last  = 0;         /* highest i for which factorial is known */

/* Ensure factorials up to n are available */
static void ensure_factorial(size_t n)
{
    if (fact_cache == NULL) {
        /* Allocate minimal space, initialise 0! = 1 */
        fact_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!fact_cache) exit(1);
        fact_cache[0] = (seq_t)1;
        fact_cap  = 1;
        fact_last = 0;
    }

    if (n >= fact_cap) {
        size_t new_cap = fact_cap ? fact_cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        fact_cache = tmp;
        fact_cap   = new_cap;
    }

    for (size_t i = fact_last + 1; i <= n; ++i) {
        /* i ≥ 1 here, fact[i-1] already valid */
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_last = n;
}

/* ---------- sequence cache ------------------------------------- */
static seq_t *seq_cache = NULL;      /* seq_cache[i] = a(i) */
static size_t seq_cap   = 0;
static size_t seq_last  = 0;

/* Ensure a[i] defined for all i ≤ n */
static void ensure_sequence(size_t n)
{
    if (seq_cache == NULL) {
        /* Allocate minimal space, set first two values */
        seq_cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!seq_cache) exit(1);
        seq_cache[0] = (seq_t)0;
        seq_cache[1] = (seq_t)1;
        seq_cap  = 2;
        seq_last = 1;
    }

    if (n >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    for (size_t i = seq_last + 1; i <= n; ++i) {
        if (i == 0) {
            seq_cache[i] = (seq_t)0;
        } else if (i == 1) {
            seq_cache[i] = (seq_t)1;
        } else {
            /* Compute a(i) = (i-1)! + (i even ? (i-2)! : 0) */
            ensure_factorial(i - 1);
            seq_t val = fact_cache[i - 1];
            if ((i & 1) == 0) {           /* i is even */
                val += fact_cache[i - 2];
            }
            seq_cache[i] = val;
        }
    }
    seq_last = n;
}

/* --------- public API ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    size_t idx = (size_t)n;
    ensure_sequence(idx);
    return seq_cache[idx];
}
