/*
 * A028355  How the astronomical clock ("Orloj") in Prague would strike
 *          1,2,3,...,24,25,.. (digits follow 12343212343... (A028356), n-th
 *          group adds to n).
 * https://oeis.org/A028355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 25, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is obtained by taking digits from the infinite
   stream 1,2,3,4,3,2,1,2,3,4,3,2,1,… and concatenating them until
   their sum equals n. This matches the OEIS definition of A028355. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*  Sequence digit sequence: 1,2,3,4,3,2,1,2,3,4,3,2,1,…  */
static const uint8_t cycle[6] = { 2, 3, 4, 3, 2, 1 };

/* Return the next digit in the infinite stream, advancing *pos. */
static inline seq_t next_digit(idx_t *pos)
{
    seq_t d;
    if (*pos == 0)
        d = 1;
    else
        d = cycle[( (*pos - 1) % 6 )];
    (*pos)++;
    return d;
}

/* Cache variables for memoisation */
static seq_t  *cache     = NULL;   /* array of computed terms */
static size_t  cache_len = 0;      /* number of terms stored */
static size_t  cache_cap = 0;      /* allocated capacity */
static idx_t   stream_pos = 0;     /* position of next digit to use */

/* Grow the cache array to at least needed length. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Main function requested by the problem */
seq_t a(idx_t n)
{
    /* Indices are 1‑based; ensure we have computed up to n. */
    while (cache_len < n)
    {
        /* Compute next term using the striking rule. */
        seq_t target = (seq_t)(cache_len + 1);
        seq_t sum    = 0;
        seq_t value  = 0;
        while (sum < target)
        {
            seq_t d = next_digit(&stream_pos);
            value = value * 10 + d;
            sum  += d;
        }
        grow_cache(cache_len + 1);
        cache[cache_len] = value;
        ++cache_len;
    }
    return cache[n - 1];
}
