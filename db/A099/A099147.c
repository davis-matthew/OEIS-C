/*
 * A099147  Iterated hexagonal numbers, starting at 1.
 * https://oeis.org/A099147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=6, a(n)=2*a(n-1)^2 - a(n-1) for n>2 (iterated
 *            hexagonal numbers)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *a_cache = NULL;   /* array of computed terms, indexed from 1 */
static idx_t a_cache_size = 0;  /* greatest index that has been computed */
static size_t a_cache_cap = 0;  /* current allocated capacity (≥ a_cache_size+1) */

/* Extend cache until it contains term a(n) */
static void ensure_cache(idx_t n)
{
    if (a_cache_size >= n) return;

    /* First allocation and seed the first two terms */
    if (a_cache_size == 0) {
        a_cache_cap = 8;
        a_cache = (seq_t*)malloc(a_cache_cap * sizeof(seq_t));
        if (!a_cache) return;          /* allocation failure: give up silently */

        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)6;
        a_cache_size = 2;
    }

    while (a_cache_size < n) {
        idx_t next = a_cache_size + 1;
        seq_t prev = a_cache[a_cache_size];
        seq_t val = (seq_t)2 * prev * prev - prev; /* recurrence */

        a_cache_size = next;
        if (a_cache_size + 1 > a_cache_cap) {     /* ensure room for the next index */
            a_cache_cap *= 2;
            a_cache = (seq_t*)realloc(a_cache, a_cache_cap * sizeof(seq_t));
            if (!a_cache) return;                 /* allocation failure: stop extending */
        }
        a_cache[a_cache_size] = val;
    }
}

/* External function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for 0 or negative indices */

    ensure_cache(n);
    return a_cache[n];
}
