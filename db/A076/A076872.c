/*
 * A076872  a(n) = number of numbers <= n that are the sum of two squarefull
 *          numbers.
 * https://oeis.org/A076872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integers m ≤ n that can be written as m = s1 +
 *            s2 where s1 and s2 are squarefull numbers. (OEIS A076872)
 * Range:     u8 n <= 423, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, gen_sqfull_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes.h"   /* build_primes() */
#include "../utils/gen_sqfull_rec.h"

/* --------------------------------------------------------------------- */
/* Integer square‑root: largest r with r² ≤ x.  Works for idx_t.           */
/* --------------------------------------------------------------------- */
static idx_t int_sqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

/* --------------------------------------------------------------------- */
/* Recursively generate all squarefull numbers ≤ limit; arrays are
   grown with realloc.  The array pointer is passed by reference.       */

/* --------------------------------------------------------------------- */
/* Generate all squarefull numbers ≤ n.  Return array via *out and count. */
/* --------------------------------------------------------------------- */
static void gen_sqfull(idx_t n, uint64_t **out, size_t *cnt)
{
    size_t cap = 0;
    uint64_t *arr = NULL;
    uint64_t limit = (uint64_t)n;

    /* Primes up to sqrt(limit) suffice for building squarefull numbers. */
    idx_t p_limit = int_sqrt(limit);
    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes(p_limit, &primes, &primes_cnt);

    gen_sqfull_rec(1, 0, primes, primes_cnt, limit, &arr, cnt, &cap);
    free(primes);
    *out = arr;
}

/* --------------------------------------------------------------------- */
/* Global state for memoization                                         */
/* --------------------------------------------------------------------- */
static seq_t *pref = NULL;     /* pref[k] = a(k) */
static idx_t cur_max = 0;

/* --------------------------------------------------------------------- */
/* Recompute prefix array up to n (inclusive).                            */
/* --------------------------------------------------------------------- */
static void recompute_up_to(idx_t n)
{
    /* Generate all squarefull numbers ≤ n */
    uint64_t *sqfull = NULL;
    size_t sqfull_cnt = 0;
    gen_sqfull(n, &sqfull, &sqfull_cnt);

    /* Mark sums of two squarefull numbers */
    bool *marks = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    for (size_t i = 0; i < sqfull_cnt; ++i) {
        for (size_t j = i; j < sqfull_cnt; ++j) {
            uint64_t s = sqfull[i] + sqfull[j];
            if (s <= (uint64_t)n) marks[(size_t)s] = true;
        }
    }

    /* Build prefix counts */
    seq_t *new_pref = (seq_t *)realloc(pref, (size_t)(n + 1) * sizeof(seq_t));
    seq_t running = 0;
    new_pref[0] = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (marks[(size_t)k]) ++running;
        new_pref[(size_t)k] = running;
    }

    free(marks);
    free(sqfull);
    pref = new_pref;
    cur_max = n;
}

/* --------------------------------------------------------------------- */
/* Public function required by the framework                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > cur_max) recompute_up_to(n);
    return pref[(size_t)n];
}
