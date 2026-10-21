/*
 * A088260  Primorials which yield palindromes if the trailing zeros are
 *          deleted.
 * https://oeis.org/A088260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = primorial_k such that primorial_k with trailing zeros
 *            removed is a decimal palindrome. a(1) = 1 (primorial(0)), a(2) =
 *            2, a(3) = 6, a(4) = 30, a(5) = 30030, a(6) = 9699690,
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   append_candidate, is_palindrome, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"        /* bool is_prime(idx_t n);          */
#include "../utils/is_palindrome.h"   /* bool is_palindrome(seq_t x, idx_t base); */
#include "../utils/append_candidate.h"/* void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val); */

seq_t a(idx_t n)
{
    /* Memoisation state */
    static bool initialized = false;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t *cache = NULL;
    static seq_t primorial = 1;    /* current primorial value */
    static uint64_t last_prime = 1; /* last prime used (1 means none yet) */

    /* Initialisation: sequence starts with 1 */
    if (!initialized) {
        append_candidate(&cache, &cache_len, &cache_cap, primorial);
        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Find next prime after last_prime */
        uint64_t candidate = last_prime + 1;
        while (!is_prime(candidate)) {
            ++candidate;
        }

        /* Update primorial */
        primorial = (seq_t)((seq_t)primorial * (seq_t)candidate);

        /* Remove trailing zeros for palindrome test */
        seq_t trimmed = primorial;
        while (trimmed % 10 == 0) {
            trimmed /= 10;
        }

        /* Check palindrome */
        if (is_palindrome(trimmed, 10)) {
            append_candidate(&cache, &cache_len, &cache_cap, primorial);
        }

        last_prime = candidate;
    }

    return cache[n - 1];
}
