/*
 * A194390  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) = 0, where
 *          r=sqrt(12) and < > denotes fractional part.
 * https://oeis.org/A194390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   m is a term iff # {k ≤ m | frac(k*sqrt(12)) < 1/2 } = m/2
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* static cache of already found terms */
static seq_t *term_cache   = NULL;   /* array of terms */
static idx_t  cache_cap    = 0;      /* capacity of term_cache   */
static idx_t  cache_size   = 0;      /* number of terms stored   */

/* current state while generating terms */
static idx_t current_m   = 0;     /* last ma­m processed      */
static idx_t count_less  = 0;     /* how many k ≤ current_m  */
                                 /* satisfy frac(k*sqrt12) < 1/2 */

/* push a new term into the cache, growing the array if needed */
static void push_term(seq_t value)
{
    if (cache_size == cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *temp = (seq_t *)realloc(term_cache,
                                       new_cap * sizeof(seq_t));
        if (!temp) exit(EXIT_FAILURE);   /* out of memory */
        term_cache = temp;
        cache_cap  = new_cap;
    }
    term_cache[cache_size++] = value;
}

/* test whether frac(k*sqrt(12)) < 1/2  */
static inline bool less_than_half(idx_t k)
{
    seq_t k2   = (seq_t)k * (seq_t)k;          /* k^2                         */
    seq_t U    = (seq_t)12 * k2;               /* 12*k^2                      */
    seq_t s    = floor_sqrt(U);                 /* s = floor(sqrt(U))          */
    seq_t left = (seq_t)4 * U;                  /* 4*U                         */
    seq_t right = (seq_t)4 * s * s + (seq_t)4 * s + (seq_t)1;
    return (left < right); /* true if fractional part < 1/2 */
}

/* OEIS sequence A194390 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defensive: offset is 1 */

    /* generate new terms until we have at least n of them */
    while (cache_size < n) {
        current_m++;
        if (less_than_half(current_m))
            count_less++;

        if ((current_m & (idx_t)1) == 0 && count_less == current_m / 2)
            push_term((seq_t)current_m);
    }

    return term_cache[n - 1];
}
