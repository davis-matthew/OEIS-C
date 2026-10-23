/*
 * A044883  Numbers having, in base 12, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑12 representation satisfies * sum of
 *            even‑length runs = sum of odd‑length runs * (OEIS A044883). The
 *            function computes the nth term * by brute‑force searching from
 *            the lowest integer upwards * and memoises found terms
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* array of found terms */
static idx_t  cache_len = 0;  /* number of terms stored */
static idx_t  cache_cap = 0;  /* capacity of the array */
static idx_t  current   = 1;  /* next candidate to test */

/* Ensure cache has room for at least 'need' elements */
static void ensure_capacity(idx_t need)
{
    if (cache_cap < need) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Test whether x satisfies the “equal run‑length” property in base 12. */
static bool has_equal_run_sums(seq_t x)
{
    /* Extract base‑12 digits (least significant first). */
    unsigned char digits[64];
    idx_t len = 0;
    seq_t tmp = x;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = (unsigned char)(tmp % 12);
            tmp /= 12;
        }
    }

    uint64_t sum_even = 0, sum_odd = 0;
    unsigned int run_len = 1;
    unsigned char prev = digits[len - 1];

    for (int i = (int)len - 2; i >= 0; --i) {
        unsigned char d = digits[i];
        if (d == prev) {
            ++run_len;
        } else {
            if (run_len % 2 == 0)
                sum_even += run_len;
            else
                sum_odd += run_len;
            run_len = 1;
            prev = d;
        }
    }
    /* Record the last run. */
    if (run_len % 2 == 0)
        sum_even += run_len;
    else
        sum_odd += run_len;

    return sum_even == sum_odd;
}

/* Public sequence function: a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined, return 0 by convention */

    /* Generate terms until we have n entries. */
    while (cache_len < n) {
        while (!has_equal_run_sums(current))
            ++current;
        /* Store found term. */
        ensure_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)current;
        ++current;
    }
    return cache[n - 1];
}
