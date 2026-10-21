/*
 * A210512  Primes formed by concatenating k, k and 3 for k >= 1.
 * https://oeis.org/A210512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 16, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   concat_parts, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210512: primes obtained by concatenating k, k and 3 for k ≥ 1
   where k is not a multiple of 3.
   The nth term is the nth prime of the form k · 10^(digits(k)+1) + k·10 + 3. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* safety for 0‑based calls */
        return (seq_t)0;

    /* static cache of already found terms and state of the generation */
    static seq_t *cache   = NULL;  /* dynamic array of found terms   */
    static size_t cache_len = 0;   /* number of terms stored        */
    static size_t cache_cap = 0;   /* allocated capacity             */
    static idx_t  next_k = 1;      /* next k to test                 */
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
    }

    /* Generate primes until we have at least n terms */
    while (cache_len < n) {
        if (next_k % 3 != 0) {                     /* skip multiples of 3 */
            seq_t cand = concat_parts(next_k, next_k);   /* k concatenated with itself */
            cand = cand * 10 + 3;                       /* append the trailing 3      */

            if (is_prime_big(cand)) {                   /* primality test             */
                if (cache_len == cache_cap) {
                    /* enlarge storage if needed */
                    ensure_capacity(cache_len + 1, &cache_cap,
                                    (void **)&cache, sizeof(seq_t));
                }
                cache[cache_len++] = cand;              /* store the prime            */
            }
        }
        next_k++;                                     /* go to next k               */
    }

    return cache[n - 1];  /* 1‑based indexing of OEIS */
}
