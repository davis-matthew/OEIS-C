/*
 * A080683  23-smooth numbers: numbers whose prime divisors are all <= 23.
 * https://oeis.org/A080683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 3526, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: 23‑smooth numbers
   a(n) is the n‑th integer whose prime factors are all ≤ 23. This
   function generates the sequence by a standard k‑smooth number
   algorithm. For each prime p in {2,3,5,7,11,13,17,19,23} it keeps an
   index of the next multiple of p that has not yet been output. The
   next term is the minimum of those candidates. Duplicates are
   avoided by advancing all indices that produce the same minimum. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static size_t prime_idx[9];
    static int init = 0;

    /* Lazy initialization of cache and prime indices */
    if (!init) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[0] = 1;                  /* first 23‑smooth number */
        cache_size = 1;
        for (int i = 0; i < 9; ++i) prime_idx[i] = 0;
        init = 1;
    }

    /* If the requested term is already available, return it. */
    if ((size_t)n <= cache_size) return cache[n - 1];

    /* Primes to be used in the generation.  They are constant for all calls. */
    static const seq_t primes[9] = {2, 3, 5, 7, 11, 13, 17, 19, 23};

    /* Grow the cache incrementally until it contains n terms. */
    while (cache_size < (size_t)n) {
        /* Extend storage if necessary. */
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failed */
            cache = tmp;
        }

        /* Find the smallest candidate among all primes. */
        seq_t min_val = cache[prime_idx[0]] * primes[0];
        for (int j = 1; j < 9; ++j) {
            seq_t val = cache[prime_idx[j]] * primes[j];
            if (val < min_val) min_val = val;
        }

        /* Append the new smallest value to the cache. */
        cache[cache_size++] = min_val;

        /* Increment all indices whose candidate equals the chosen minimum
           to avoid duplicates in subsequent iterations. */
        for (int j = 0; j < 9; ++j) {
            seq_t val = cache[prime_idx[j]] * primes[j];
            if (val == min_val) ++prime_idx[j];
        }
    }

    return cache[n - 1];
}
