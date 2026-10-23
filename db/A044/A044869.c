/*
 * A044869  Positive integers having the same number of base-13 runs of odd
 *          length as even.
 * https://oeis.org/A044869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS sequence A044869:
   numbers whose base‑13 digit runs contain an equal number of odd‑length
   and even‑length runs. The function performs a brute‑force search
   starting from 1, caching results so that repeated calls are efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Static cache for already computed terms */
static seq_t *cache = NULL;      /* array of computed a(k) values */
static idx_t   cache_len = 0;    /* number of elements stored */
static idx_t   cache_cap = 0;    /* capacity of the cache */

/* Ensure the cache has space for at least 'need' more elements. */
static void grow_cache(idx_t need) {
    if (cache_len + need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < cache_len + need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache)
        memcpy(new_arr, cache, cache_len * sizeof(seq_t));
    free(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

/* Test whether a number satisfies the base‑13 run‑parity condition. */
static bool has_equal_odd_even_runs(seq_t n) {
    if (n == 0)
        return false;

    /* Extract base‑13 digits (least significant first). */
    int digits[256];       /* maximum digits needed for 128‑bit numbers */
    idx_t len = 0;
    seq_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 13);
        tmp /= 13;
    }

    /* Count runs of odd and even length. */
    int odd = 0, even = 0;
    int curr  = digits[0];
    int runlen = 1;

    for (idx_t i = 1; i < len; ++i) {
        if (digits[i] == curr) {
            runlen++;
        } else {
            if (runlen & 1)
                odd++;
            else
                even++;
            curr   = digits[i];
            runlen = 1;
        }
    }
    /* Final run */
    if (runlen & 1)
        odd++;
    else
        even++;

    return odd == even;
}

/* Main function required by the test harness. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;           /* Zero is not part of the sequence. */

    /* If we already have the answer cached, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Find additional terms until we reach term n. */
    seq_t candidate = cache_len ? cache[cache_len - 1] + 1 : 1;
    while (cache_len < n) {
        if (has_equal_odd_even_runs(candidate)) {
            grow_cache(1);
            cache[cache_len++] = candidate;
        }
        candidate++;
    }

    return cache[n - 1];
}
