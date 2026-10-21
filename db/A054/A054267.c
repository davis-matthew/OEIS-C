/*
 * A054267  Sum of composite numbers between prime p and nextprime(p) is
 *          palindromic with restriction 'p + 1 <> sum'.
 * https://oeis.org/A054267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   is_palindrome, is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the nth prime p (offset 0) for which the sum of
 * composite numbers strictly between p and the next prime after p is a
 * palindromic decimal number and is not equal to p + 1.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_small.h"
#include "../utils/is_palindrome.h"

static bool check_prime(uint64_t p, seq_t *out)
{
    /* Next prime strictly greater than p */
    uint64_t q = next_prime(p);
    if (q <= p) return false;          /* sanity */

    uint64_t count = q - p - 1;
    if (count == 0) return false;       /* no integers between p and q */

    seq_t sumComp = 0;
    for (uint64_t x = p + 1; x < q; ++x) {
        if (is_prime_small(x)) continue;
        sumComp += (seq_t)x;
    }

    /* Restriction: sum must not equal p + 1 */
    if (sumComp == (seq_t)(p + 1)) return false;

    /* Sum must be a palindrome in base 10 */
    if (!is_palindrome(sumComp, (idx_t)10)) return false;

    *out = (seq_t)p;
    return true;
}

/* Cache of computed sequence elements */
static seq_t *cache    = NULL;
static size_t cache_len = 0;      /* number of valid entries */
static size_t cache_cap = 0;
static uint64_t last_checked = 0; /* last prime examined (exclusive) */

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }
    while (cache_len <= (size_t)n) {
        /* Find next prime after the last one we looked at */
        uint64_t p = next_prime(last_checked + 1);
        last_checked = p;

        seq_t val;
        if (check_prime(p, &val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        /* If prime did not satisfy the condition, keep looping */
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
