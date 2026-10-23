/*
 * A011727  A binary m-sequence: expansion of reciprocal of x^14 + x^12 + x^11
 *          + x + 1 (mod 2, shifted by 13 initial 0's).
 * https://oeis.org/A011727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9764, u16 n <= 9764, u32 n <= 9764, u64 n <= 9764,
 *            u128 n <= 9764, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A011727: a binary m‑sequence derived from 1/(x^14 + x^12 + x^11 + x + 1) over GF(2).
   The terms satisfy the linear recurrence
        a(n) = a(n-1) XOR a(n-11) XOR a(n-12) XOR a(n-14)
   with initial values a(0)…a(12) = 0 and a(13) = 1.
   The sequence is 16383‑periodic. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* lazily initialized cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* number of terms stored (last index + 1) */

    /* first initialisation */
    if (cache == NULL) {
        cap = 14;                     /* indices 0 … 13 available */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        for (size_t i = 0; i < 13; ++i) cache[i] = (seq_t)0; /* a(0)…a(12) = 0 */
        cache[13] = (seq_t)1;          /* a(13) = 1 */
    }

    /* extend cache up to index n if necessary */
    while (cap <= (size_t)n) {
        size_t idx = cap;                      /* next index to compute */

        /* recurrence calculation */
        seq_t val = cache[idx-1] ^ cache[idx-11] ^
                    cache[idx-12] ^ cache[idx-14];
        val &= (seq_t)1;                       /* keep only the least significant bit */

        /* grow the cache by one element */
        seq_t *new_cache = (seq_t *)realloc(cache, (cap + 1) * sizeof(seq_t));
        if (new_cache == NULL) exit(EXIT_FAILURE);
        cache = new_cache;
        cache[idx] = val;

        ++cap;
    }

    return cache[(size_t)n];
}
