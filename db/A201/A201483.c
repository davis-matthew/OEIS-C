/*
 * A201483  primes of the form 5n^2 + 4.
 * https://oeis.org/A201483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 982, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 5·n² + 4.
   The n-th term a(n) is the n-th prime of the form 5*k² + 4
   (k is the integer that generates the prime). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"

static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *new_arr = seq_realloc(*arr, *cap, new_cap);
        *arr = new_arr;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Lazy static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static idx_t next_k = 1;   /* next integer k to test */

    if (n == 0) return 0; /* undefined for n = 0, but avoid UB */

    if ((size_t)n <= cache_cnt) {
        return cache[n - 1];
    }

    grow_array(&cache, &cache_cap, (size_t)n);

    while (cache_cnt < n) {
        seq_t k = (seq_t)next_k;
        seq_t p = 5 * k * k + 4;
        if (isPrimeGeneric(p)) {
            cache[cache_cnt++] = p;
        }
        ++next_k;
    }

    return cache[n - 1];
}
