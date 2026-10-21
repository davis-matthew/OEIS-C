/*
 * A202266  Composite numbers in which all substrings and reversal substrings
 *          are composites.
 * https://oeis.org/A202266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer such that the integer is composite and
 *            every contiguous decimal substring (including the whole number)
 *            and its reversals are also composite. The sequence is enumerated
 *            by brute‑force search with memoisation
 * Range:     u8 n <= 18, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   isPrimeGeneric, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_to_str.h"

/* Static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 0;

/* Test whether a number and all of its substrings and
   their reversal substrings are composite. */
static bool all_substrings_composite(seq_t x)
{
    /* Whole number must be composite (>= 4 and not prime). */
    if (x <= 3 || isPrimeGeneric(x))
        return false;

    /* Convert the number to a terminating string of its digits. */
    /* 4096 digits is ample for all numbers sought up to n ≈ 10000. */
    static char buf[4096];
    size_t len;
    seq_to_str(x, buf, &len);

    /* Iterate over every contiguous substring. */
    for (size_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            val = val * 10 + (buf[j] - '0');
            if (val <= 3 || isPrimeGeneric(val))
                return false;

            /* Reversed substring buf[i..j] */
            seq_t rev = 0;
            for (size_t k = j; k > i; --k) {
                rev = rev * 10 + (buf[k] - '0');
            }
            rev = rev * 10 + (buf[i] - '0');
            if (rev <= 3 || isPrimeGeneric(rev))
                return false;
        }
    }
    return true;
}

/* Ensure the cache has at least 'need' entries. */
static void ensure_cache_size(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_mem)
        exit(EXIT_FAILURE);   /* Out of memory */
    cache = new_mem;
    cache_cap = new_cap;
}

/* Compute the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    size_t target = (size_t)n;
    while (cache_len < target) {
        /* Candidate numbers start at 4 (the smallest composite). */
        if (++last_checked < 4)
            last_checked = 4;
        if (all_substrings_composite(last_checked)) {
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
    }
    return cache[target - 1];
}
