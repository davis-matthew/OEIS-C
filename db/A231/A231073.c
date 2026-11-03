/*
 * A231073  Primes whose English name has a prime number of words.
 * https://oeis.org/A231073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A231073: primes whose English name has a prime number of words.
 * The function returns the n‑th such prime (1‑indexed) by brute‑force search.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* number of words in the English name for the last 3 digits (0–999) */
static uint32_t
count_words_group(uint32_t g)
{
    uint32_t cnt = 0;
    if (g >= 100) {
        cnt += 1;            /* word for the hundreds digit */
        cnt += 1;            /* "hundred" */
        g %= 100;
    }
    if (g > 0) {
        if (g < 20) {
            cnt += 1;        /* single word for 1–19 */
        } else {
            cnt += 1;        /* tens word (twenty, thirty, …) */
            if (g % 10 != 0) {
                cnt += 1;    /* unit word */
            }
        }
    }
    return cnt;
}

/* total number of words in the full English name of n (n > 0) */
static uint32_t
word_count_uint64(uint64_t n)
{
    uint32_t total = 0;
    uint32_t scale_idx = 0;        /* 0 → units, 1 → thousand, 2 → million, … */

    while (n > 0) {
        uint32_t grp = (uint32_t)(n % 1000);
        n /= 1000;
        if (grp != 0) {
            total += count_words_group(grp);
            if (scale_idx > 0) {
                total += 1;        /* scale word: thousand, million, … */
            }
        }
        ++scale_idx;
    }
    return total;
}

/* a(n): n‑th prime whose English name has a prime number of words */
seq_t
a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoized sequence values */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_candidate = 2;   /* next number to test (prime candidates) */

    if (n == 0) return 0;   /* undefined, but harmless */

    /* return cached value if already computed */
    if (cache_len >= (size_t)n) {
        return cache[(size_t)n - 1];
    }

    /* generate until we have the n-th term */
    while (cache_len < (size_t)n) {
        if (is_prime(next_candidate)) {
            uint32_t wc = word_count_uint64(next_candidate);
            if (is_prime((idx_t)wc)) {        /* word count is prime */
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) return 0;        /* allocation failed – abort */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
