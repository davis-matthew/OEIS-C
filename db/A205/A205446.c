/*
 * A205446  s(k)-s(j), where (k,j) is the least pair for which n divides
 *          s(k)-s(j), and s(j)=Fibonacci(2j-1).
 * https://oeis.org/A205446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 24, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A205446
   a(n) = the smallest positive difference s(k)-s(j) (k>j≥1)
   such that n divides s(k)-s(j),
   where s(t) = Fibonacci(2t‑1).  The term a(1) corresponds to
   the first non‑zero difference, and a(n) is defined for every n≥1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache for already computed indices. */
static seq_t *a_cache = NULL;
static bool   *a_comp  = NULL;
static idx_t   a_cap   = 0;          /* current capacity of the cache */

/* enlarge cache so that indices 1…n can be stored */
static void
ensure_cache(idx_t n)
{
    if (a_cap <= n) {
        idx_t new_cap = a_cap ? a_cap * 2 : n + 1;
        if (new_cap < n + 1) new_cap = n + 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_comp  = (bool  *)realloc(a_comp,  new_cap * sizeof(bool));
        for (idx_t i = a_cap; i < new_cap; ++i) {
            a_cache[i] = (seq_t)0;
            a_comp[i]  = false;
        }
        a_cap = new_cap;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset starts at 1 */

    ensure_cache(n);

    if (a_comp[n]) return a_cache[n];    /* already computed */

    /* array that remembers the first occurrence of each residue modulo n.
       0 means "not seen yet".  The value stored is a 1‑based index.   */
    idx_t *first = (idx_t*)calloc((size_t)n, sizeof(idx_t));
    if (!first) { a_cache[n] = (seq_t)0; a_comp[n] = true; return a_cache[n]; }

    /* array to hold the sequence values s(t) = Fibonacci(2t-1). */
    size_t cap   = 1024;
    seq_t  *s_arr = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!s_arr) { free(first); a_cache[n] = (seq_t)0; a_comp[n] = true; return a_cache[n]; }
    size_t len = 0;

    seq_t s_prev2 = (seq_t)0;   /* will hold s(t-2)   */
    seq_t s_prev1 = (seq_t)0;   /* will hold s(t-1)   */

    for (idx_t t = 1; ; ++t) {
        seq_t s_cur;
        if (t == 1)          s_cur = (seq_t)1;          /* F1  */
        else if (t == 2)     s_cur = (seq_t)2;          /* F3  */
        else                 s_cur = 3 * s_prev1 - s_prev2; /* recurrence for odd indices */

        /* store current value */
        if (len == cap) {
            cap <<= 1;
            s_arr = (seq_t*)realloc(s_arr, cap * sizeof(seq_t));
            if (!s_arr) break; /* allocation failed – terminate */
        }
        s_arr[len++] = s_cur;

        /* residue modulo n */
        idx_t res = (idx_t)(s_cur % (seq_t)n);

        if (first[res] != 0) {            /* repetition found */
            idx_t j = first[res] - 1;     /* 0‑based index of earlier occurrence */
            seq_t diff = s_cur - s_arr[j];
            a_cache[n] = diff;
            a_comp[n]  = true;
            free(first);
            free(s_arr);
            return diff;
        } else {
            first[res] = t;               /* record first occurrence (1‑based) */
        }

        /* shift for next iteration */
        s_prev2 = s_prev1;
        s_prev1 = s_cur;
    }

    /* should never reach here, but fall back gracefully */
    free(first);
    free(s_arr);
    a_cache[n] = (seq_t)0;
    a_comp[n]  = true;
    return a_cache[n];
}
