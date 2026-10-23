/*
 * A192173  Numbers whose writing in every base from 2 to 10 requires all ten
 *          (0-9) digits at least once.
 * https://oeis.org/A192173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) computed by exhaustive search; no closed form known
 * Range:     u8 n <= 4, u16 n <= 8746, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Test whether a number contains all ten decimal digits 0‑9 in each
 * bases 2 through 10. */
static bool has_all_digits(seq_t n)
{
    unsigned mask = 0;
    for (int base = 2; base <= 10; ++base)
    {
        seq_t temp = n;
        if (temp == (seq_t)0)
        {
            mask |= 1u << 0;
            continue;
        }
        while (temp > (seq_t)0)
        {
            unsigned digit = (unsigned)(temp % base);
            mask |= 1u << digit;
            temp = temp / base;
        }
    }
    return mask == 0x3FFu;   /* bits 0‑9 all set */
}

/* Simple dynamic cache for the sequence values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_candidate = 1;

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* OEIS A192173: numbers that in every base from 2 to 10 use all
 * decimal digits at least once. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, return 0 */
    while (cache_len < (size_t)n)
    {
        if (has_all_digits(next_candidate))
        {
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        next_candidate = next_candidate + (seq_t)1;
    }
    return cache[n - 1];
}
