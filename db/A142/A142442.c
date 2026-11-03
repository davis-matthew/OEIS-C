/*
 * A142442  Primes congruent to 33 mod 49.
 * https://oeis.org/A142442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) are primes congruent to 33 mod 49. Implemented by
 *            incremental search
 * Range:     u8 n <= 2, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 915 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;
static size_t cap = 0;
static size_t count = 0;
static idx_t next_k = 2; /* corresponds to 33 + 49*next_k */

static void grow_cache(size_t needed)
{
    if (needed <= cap) return;
    size_t new_cap = cap ? cap * 2 : needed;
    seq_t *newp = seq_realloc(cache, cap, new_cap);
    cache = newp;
    cap = new_cap;
}

/* Return the nth prime congruent to 33 mod 49 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but safe */

    while (count < n) {
        idx_t cand = 33 + 49 * next_k;    /* candidate number */
        if (is_prime(cand)) {
            grow_cache(count + 1);
            cache[count++] = (seq_t)cand;
        }
        next_k++;
    }
    return cache[n - 1];
}
