/*
 * A238332  Primes whose digits, after the first, form a nonprime number not
 *          occurring earlier.
 * https://oeis.org/A238332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1938, u32 n <= 2283, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, is_prime_big, pow10, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A238332: primes whose digits after the first form a
   non‑prime number that has not appeared previously.
   The function generates terms on demand and caches them for
   subsequent calls.  It runs in O(n²) time in the worst case, which
   is easily fast enough for n ≤ 10 000. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */
#include "../utils/digit_len.h"     /* int digit_len(seq_t x) */
#include "../utils/pow10.h"         /* seq_t pow10(idx_t e) */
#include "../utils/seq_realloc.h"   /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

/* Append value to a dynamic array of seq_t. */
static void append_seq_t(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        *arr = (seq_t *)seq_realloc(*arr, *cap, new_cap);
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Cached terms of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Cached suffixes that have already appeared */
    static seq_t *suffixes = NULL;
    static size_t suffix_len = 0;
    static size_t suffix_cap = 0;

    /* Next integer to test for primality */
    static uint64_t candidate = 2;

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t cand_seq = (seq_t)candidate;
        if (is_prime_big(cand_seq)) {
            int d = digit_len(cand_seq);
            if (d > 1) {
                seq_t p10 = pow10((idx_t)(d - 1));
                seq_t suffix = cand_seq % p10;
                if (!is_prime_big(suffix)) {
                    bool seen = false;
                    for (size_t i = 0; i < suffix_len; ++i) {
                        if (suffixes[i] == suffix) {
                            seen = true;
                            break;
                        }
                    }
                    if (!seen) {
                        append_seq_t(&cache, &cache_len, &cache_cap, cand_seq);
                        append_seq_t(&suffixes, &suffix_len, &suffix_cap, suffix);
                    }
                }
            }
        }
        candidate++;
    }

    return cache[(size_t)n - 1];
}
