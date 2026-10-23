/*
 * A118706  Triangular numbers whose digits in base 12 are contained in
 *          {1,5,7,11}.
 * https://oeis.org/A118706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Triangular numbers whose base‑12 digits belong to {1,5,7,11}.
   The n‑th term is the n‑th integer k such that T(k)=k*(k+1)/2 has only
   allowed digits in base 12. */
#include <stdlib.h>
#include <stdbool.h>

/* Check that all base‑12 digits of x are in {1,5,7,11}. */
static bool allowed_digits(seq_t x)
{
    while (x > 0) {
        unsigned d = (unsigned)(x % 12);
        if (d != 1 && d != 5 && d != 7 && d != 11)
            return false;
        x /= 12;
    }
    return true;
}

/* Cache for the sequence values.  The array grows lazily as needed. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;   /* number of stored terms */
static idx_t  next_m   = 1;    /* next m to test for a new term */

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)        /* Sequence starts at index 1. */
        return (seq_t)0;

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t T = ((seq_t)next_m) * ((seq_t)next_m + 1) / 2;
        if (allowed_digits(T)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                /* reallocate; cast to suppress C++ warning */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = T;
        }
        next_m++;
    }

    return cache[(size_t)n - 1];
}
