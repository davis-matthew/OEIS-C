/*
 * A144591  Composites of the form smallest digit of n + prime(n).
 * https://oeis.org/A144591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime, is_prime_big, min_digit_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A144591
 *  a(n) = smallest digit of k + prime(k) for the n-th k such that
 *  the sum is composite.
 *  The sequence lists the composite values in the order of increasing k.
 *
 *  Implementation details:
 *  - k is iterated from 1 upwards.
 *  - For each k we compute the smallest decimal digit (0‑9).
 *  - We obtain the k-th prime via get_prime(k).
 *  - The sum is tested for compositeness with is_prime_big.
 *  - Results are cached so any order of calls works efficiently.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */
#include "../utils/min_digit_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */

    /* ----- static cache of computed composite sums ----- */
    static seq_t *cache      = NULL;      /* array of composite sums      */
    static size_t cache_len = 0;          /* number of stored terms        */
    static size_t cache_cap = 0;          /* allocated slots                */
    static idx_t  k_next    = 1;          /* next k to process             */

    /* ---- grow cache until we have n terms ---- */
    while (cache_len < (size_t)n) {
        /* Compute smallest digit of k_next */
        unsigned md = min_digit_idx(k_next);

        /* Get the k_next-th prime (1‑based). */
        uint32_t pk = get_prime(k_next);

        /* Sum = smallest digit + prime(k). */
        seq_t sum = (seq_t)md + (seq_t)pk;

        /* If sum is composite, store it. */
        if (!is_prime_big(sum)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sum;
        }

        ++k_next;
    }

    return cache[(size_t)n - 1];
}
