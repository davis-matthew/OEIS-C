/*
 * A090933  Primes such that most significant digit swapped with all other
 *          digits yields primes.
 * https://oeis.org/A090933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digits_to_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: primes p such that swapping the most significant digit
   with any other digit yields a prime (for every other digit). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/digits_to_array.h"

/* Cache of already found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Test whether a prime n satisfies the digit‑swap property. */
static bool is_good(seq_t n)
{
    /* 0‑ or 1‑digit primes trivially satisfy the property. */
    if (n < (seq_t)10) return true;

    uint8_t buf[32];          /* enough for 256‑digit numbers */
    size_t len, i;
    digits_to_array(n, buf, &len);

    uint8_t msd_index = len - 1;          /* index of most significant digit */
    uint8_t msd = buf[msd_index];

    /* For each other digit, swap with the msd and test primality. */
    for (uint8_t pos = 0; pos < msd_index; ++pos) {
        /* swap digits */
        uint8_t tmp = buf[pos];
        buf[pos]     = msd;
        buf[msd_index] = tmp;

        /* rebuild the swapped number */
        seq_t val = (seq_t)0;
        for (size_t k = msd_index; k < len; --k) {
            val = val * (seq_t)10 + (seq_t)buf[k];
        }

        if (!is_prime_big(val)) {
            /* restore original digits before returning */
            buf[msd_index] = msd;
            buf[pos]       = tmp;
            return false;
        }

        /* restore original digits */
        buf[msd_index] = msd;
        buf[pos]       = tmp;
    }
    return true;
}

/* Extend the cache until it contains at least 'need' terms. */
static void ensure_terms(idx_t need)
{
    while (cache_len < need) {
        seq_t cand = (cache_len == 0) ? (seq_t)2 : cache[cache_len - 1] + (seq_t)1;

        /* Skip even numbers beyond 2. */
        if (cand != (seq_t)2) {
            while ((cand % (seq_t)2) == (seq_t)0)
                ++cand;
        }

        /* Search for the next qualifying prime. */
        while (true) {
            if (is_prime_big(cand) && is_good(cand)) {
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    cache = tmp;
                }
                cache[cache_len++] = cand;
                break;
            }
            cand += (seq_t)2;   /* skip to the next odd candidate */
        }
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 – behaviour below offset is irrelevant */
    ensure_terms(n);
    return cache[n - 1];
}
