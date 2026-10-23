/*
 * A179036  Emirps whose only prime digits are 5's.
 * https://oeis.org/A179036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 5, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   get_digits, is_prime_big, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A179036: Emirps whose only prime digits are 5's. */
/* The function enumerates decimal integers, keeps those whose digits
   contain no 2,3,7 and contain at least one 5, then tests primality
   of the number and its reverse (which must be a different prime).
   Results are cached so that calls can be in any order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t); */
#include "../utils/reverse_decimal.h"/* seq_t reverse_decimal(seq_t); */
#include "../utils/get_digits.h"    /* void get_digits(seq_t, uint8_t*, size_t*); */

static seq_t *cache = NULL;      /* stored Emirps */
static size_t cache_len = 0;     /* number of stored terms */
static size_t cache_cap = 0;     /* allocated capacity */
static seq_t max_checked = 0;    /* last integer examined */

/* Grow the cache when capacity is exhausted. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return true if x contains only digits 0-9 with no 2,3,7 and
   contains at least one digit 5. */
static bool digits_ok(seq_t x)
{
    uint8_t buf[200] = { 0 };
    size_t len = 0;
    get_digits(x, buf, &len);

    bool has5 = false;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = buf[i];
        if (d == 2 || d == 3 || d == 7) return false;
        if (d == 5) has5 = true;
    }
    return has5;
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 */

    /* Enumerate until we have at least n terms. */
    while (cache_len < (size_t)n) {
        ++max_checked;

        /* Quick digit test. */
        if (!digits_ok(max_checked))
            continue;

        /* Must be prime. */
        if (!is_prime_big(max_checked))
            continue;

        /* Reverse must be a distinct prime. */
        seq_t rev = reverse_decimal(max_checked);
        if (rev == max_checked)          /* palindrome prime excluded */
            continue;
        if (!is_prime_big(rev))
            continue;

        /* Store the emirp. */
        if (cache_len == cache_cap)
            grow_cache();
        cache[cache_len++] = max_checked;
    }

    return cache[n - 1];
}
