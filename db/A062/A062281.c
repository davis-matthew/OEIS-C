/*
 * A062281  Smallest multiple of n-th prime with all even digits.
 * https://oeis.org/A062281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 94, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   all_even_digits, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A062281: The smallest multiple of the n‑th prime whose decimal
   representation contains only even digits (0,2,4,6,8).  The sequence
   is computed lazily and memoised; each term is searched by
   multiplying the prime by even integers until all digits are even. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/all_even_digits.h"

seq_t a(idx_t n)
{
    /* Memoisation storage for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure the cache can hold the requested index */
    if (n > cache_len) {
        size_t req = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, req * sizeof(seq_t));
        if (!tmp) { free(cache); exit(EXIT_FAILURE); }  /* allocation failed */
        cache = tmp;

        /* Compute missing terms */
        for (size_t i = cache_len; i < req; ++i) {
            /* n is 1‑based, get_prime is 1‑based */
            uint32_t p_int = (uint32_t)get_prime((idx_t)(i + 1));
            seq_t prime = (seq_t)p_int;
            seq_t result;

            if (prime == (seq_t)2) {
                /* The first prime 2 itself satisfies the condition */
                result = (seq_t)2;
            } else {
                seq_t k = (seq_t)2;       /* start with the smallest even multiplier */
                while (true) {
                    seq_t cand = prime * k;
                    if (all_even_digits(cand)) {
                        result = cand;
                        break;
                    }
                    k += (seq_t)2;        /* only even multipliers can give an even last digit */
                }
            }
            cache[i] = result;
        }
        cache_len = req;
    }
    return cache[(size_t)n - 1];
}
