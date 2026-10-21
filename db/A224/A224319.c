/*
 * A224319  Primes without "1" as a digit that remain prime when any single
 *          digit is replaced with "1".
 * https://oeis.org/A224319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 18, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_cache, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search of primes lacking the digit 1
   that keep prime when any single digit is replaced by 1. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/alloc_cache.h"
#include "../utils/is_prime_big.h"

/* Fixed cache capacity large enough for n <= 10000. */
#define CACHE_CAP 20000U

/* Static cache of found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of range */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache = alloc_cache(CACHE_CAP);
        if (cache == NULL) exit(EXIT_FAILURE); /* allocation failed */
    }

    /* generate new terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t candidate = 2 + (seq_t)cache_len; /* crude start hint */
        /* improve start: skip below smallest cached term */
        if (cache_len > 0) candidate = cache[cache_len - 1] + 1;

        /* search from last checked number */
        static seq_t last_checked = 2;
        if (candidate < last_checked) candidate = last_checked;
        while (true) {
            /* extract digits of candidate (LSB first) */
            seq_t tmp = candidate;
            unsigned char dig[32];
            size_t len = 0;
            while (tmp > 0) {
                dig[len++] = (unsigned char)(tmp % 10U);
                tmp /= 10U;
            }
            if (len == 0) { len = 1; dig[0] = 0; }

            /* skip if any digit is 1 */
            bool has_one = false;
            for (size_t i = 0; i < len; ++i) {
                if (dig[i] == 1U) { has_one = true; break; }
            }
            if (has_one) { ++candidate; continue; }

            /* skip if candidate is not prime */
            if (!is_prime_big(candidate)) { ++candidate; continue; }

            /* test all single‑digit replacements with 1 */
            bool ok = true;
            for (size_t pos = 0; pos < len && ok; ++pos) {
                seq_t val = 0;
                for (int k = (int)len - 1; k >= 0; --k) {
                    unsigned char d = dig[k];
                    if ((size_t)k == pos) d = 1U;
                    val = val * 10 + d;
                }
                if (!is_prime_big(val)) ok = false;
            }
            if (ok) {
                cache[cache_len++] = candidate;
                break;
            }
            ++candidate;
        }
        last_checked = candidate + 1;
    }
    return cache[n - 1];
}
