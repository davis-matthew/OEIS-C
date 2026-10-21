/*
 * A084988  Numbers made with nonprime digits such that the sum of the digits
 *          is also not prime.
 * https://oeis.org/A084988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth number composed of digits {0,1,4,6,8,9} whose digit
 *            sum is nonprime
 * Range:     u8 n <= 46, u16 n <= 3250, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   digit_sum, get_digits, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/get_digits.h"
#include "../utils/digit_sum.h"
#include "../utils/isPrimeTrial.h"

/* Table of allowed digits: 0,1,4,6,8,9 are true; others false. */
static const bool allowed[10] = {
    true,  /* 0 */
    true,  /* 1 */
    false, /* 2 */
    false, /* 3 */
    true,  /* 4 */
    false, /* 5 */
    true,  /* 6 */
    false, /* 7 */
    true,  /* 8 */
    true   /* 9 */
};

/* Cache for already generated terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of stored terms  */
static size_t cache_cap = 0;   /* allocated capacity       */
static seq_t last_checked = 0; /* largest integer that has already been examined */

/* Resize the cache array while copying existing elements using assignment. */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) { /* keep old cache on allocation failure */
        return;
    }
    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];
    if (cache)
        free(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

/* Append a new value to the cache, ensuring capacity. */
static void push_cache(seq_t val)
{
    ensure_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* Return true if all digits of x are allowed and the digit sum is non‑prime. */
static bool is_good(seq_t x)
{
    size_t len;
    uint8_t buf[32]; /* large enough for all practical seq_t values */
    get_digits(x, buf, &len);

    for (size_t i = 0; i < len; ++i) {
        uint8_t d = buf[i];
        if (!allowed[d])
            return false; /* contains a prime digit */
    }

    seq_t s = digit_sum(x);
    return !isPrimeTrial(s); /* true when digit sum is not prime */
}

/* Generate terms until at least n terms are stored. */
static void generate_up_to(size_t n)
{
    if (cache_len >= n)
        return;

    seq_t cur = last_checked + 1;
    while (cache_len < n) {
        if (is_good(cur))
            push_cache(cur);
        ++cur;
    }
    last_checked = cur - 1;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined offset; safe default */
    generate_up_to((size_t)n);
    return cache[(size_t)n - 1];
}
