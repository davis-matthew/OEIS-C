/*
 * A193413  Valley numbers.
 * https://oeis.org/A193413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth valley number. Computed by a sequential search
 *            with a small cache
 * Range:     u8 n <= 23, u16 n <= 989, u32 n <= 9159, u64 n <= 9159,
 *            u128 n <= 9159, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* test whether v is a valley number.  Works for both signed/unsigned
   integer seq_t as well as for an arbitrary‑precision class with overloaded
   arithmetic operators. */
static bool is_valley(seq_t v)
{
    if (v == 0)
        return false;                  /* first digit must be 1–9 */

    /* enough digits for any number that can appear for n ≤ 10000 */
    uint8_t digits[128];
    size_t len = 0;

    seq_t t = v;
    while (t > 0) {
        digits[len++] = (uint8_t)(t % 10);
        t /= 10;
    }

    /* reverse to most significant first */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t tmp = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = tmp;
    }

    /* first digit equals last digit (both 1–9) */
    if (digits[0] == 0 || digits[0] != digits[len - 1])
        return false;

    /* find minimal digit */
    uint8_t min_digit = digits[0];
    for (size_t i = 1; i < len; ++i)
        if (digits[i] < min_digit) min_digit = digits[i];

    /* nonincreasing up to the first occurrence of the minimum digit */
    size_t i = 0;
    while (i + 1 < len && digits[i] >= digits[i + 1])
        ++i;

    /* we must be at a minimum */
    if (digits[i] != min_digit)
        return false;

    /* skip any plateau of the minimum digit */
    while (i + 1 < len && digits[i + 1] == min_digit)
        ++i;

    /* remaining digits must be nondecreasing */
    for (size_t j = i; j + 1 < len; ++j)
        if (digits[j] > digits[j + 1])
            return false;

    return true;
}

/* a(n) returns the nth valley number (1‑based).  Results are memoised
   so that successive or arbitrary calls are fast. */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined but harmless */
        return 0;

    /* static cache for memoisation */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_val   = 1;

    /* use cached value if already computed */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* grow cache to fit n entries */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* out‑of‑memory: abort */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate valley numbers until the n‑th one is found */
    while (cache_len < (size_t)n) {
        seq_t v = next_val;
        while (!is_valley(v))
            ++v;
        cache[cache_len++] = v;
        next_val = v + 1;
    }

    return cache[n - 1];
}
