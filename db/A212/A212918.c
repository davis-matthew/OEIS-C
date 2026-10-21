/*
 * A212918  Numbers whose sum of prime factors (counted with multiplicity) is
 *          a pentagonal number (A000326).
 * https://oeis.org/A212918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth integer k (k ≥ 1) whose sum of prime factors (with
 *            multiplicity) equals a pentagonal number (P(j) = j(3j-1)/2)
 * Range:     u8 n <= 19, u16 n <= 1428, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   sum_of_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: seq_t and idx_t are defined by the build system. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sum_of_prime_factors.h"

/* integer square root for 128‑bit unsigned integers ---------------------------------- */
static uint64_t isqrt128(unsigned __int128 n)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64) - 1;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == n) return (uint64_t)mid;
        if (sq < n)  low = mid + 1;
        else         high = mid - 1;
    }
    return (uint64_t)high;
}

/* check whether s is a pentagonal number -------------------------------------------- */
static bool is_pentagonal(uint64_t s)
{
    if (s == 0) return true;  /* P(0) = 0 */
    unsigned __int128 d = (unsigned __int128)s * 24 + 1;
    uint64_t r = isqrt128(d);
    return (unsigned __int128)r * r == d && ((r + 1) % 6) == 0;
}

/* static cache of found terms ------------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of seq_t values */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_size = 0;      /* number of stored terms */
static uint64_t cur      = 1;      /* next candidate number to test */

/* ensure that cache holds at least the first n+1 terms (0‑based) --------------------- */
static void ensure_cache_for(idx_t n)
{
    while (cache_size <= (size_t)n) {
        if (is_pentagonal(sum_of_prime_factors(cur))) {
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); abort(); }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)cur;
        }
        ++cur;
    }
}

/* required function --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n = 0 */
    ensure_cache_for(n - 1);
    return cache[n - 1];
}
