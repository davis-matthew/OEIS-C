/*
 * A092623  Primes with exactly three prime digits.
 * https://oeis.org/A092623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime that has exactly three prime digits
 *            (2,3,5,7)
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence on demand, memoising the values.
   It relies on the helper function is_prime_big from the build utilities. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Append a value to a dynamically growing array. */
static void append_value(seq_t **array, size_t *len, size_t *cap, seq_t value)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*array, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        *array = tmp;
        *cap = new_cap;
    }
    (*array)[*len] = value;
    (*len)++;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    /* Offset is 1: n == 0 should never be requested, but return 0 for safety. */
    if (n == 0) return 0;

    /* Static memoisation state. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;  /* number of cached terms (>=0) */
    static size_t cache_cap  = 0;  /* allocated capacity */
    static seq_t next_candidate = 223;  /* first odd prime with 3 prime digits */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Find the next prime with exactly three prime digits. */
        while (1) {
            /* Quick digit filter: count prime digits. */
            seq_t tmp = next_candidate;
            unsigned prime_digit_cnt = 0;
            while (tmp > 0) {
                unsigned d = (unsigned)(tmp % 10);
                if (d == 2 || d == 3 || d == 5 || d == 7) prime_digit_cnt++;
                tmp /= 10;
            }
            if (prime_digit_cnt == 3 && is_prime_big(next_candidate))
                break; /* found the next required term */

            /* Advance to the next odd candidate. */
            next_candidate += 2;
        }

        /* Store the found term. */
        append_value(&cache, &cache_len, &cache_cap, next_candidate);

        /* Advance past the found prime for the next search. */
        next_candidate += 2;
    }

    return cache[(size_t)n - 1];
}
