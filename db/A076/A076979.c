/*
 * A076979  Deleting exactly one digit yields a prime.
 * https://oeis.org/A076979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 152, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that become prime when exactly one decimal digit is deleted */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../utils/is_prime_big.h"

/* Return true if deleting exactly one decimal digit from x yields a prime. */
static bool qualifies(idx_t x) {
    uint8_t digits[32];
    size_t len = 0;

    /* Extract digits in most‑significant‑first order. */
    idx_t t = x;
    if (t == 0) {
        digits[len++] = 0;
    } else {
        uint8_t rev[32];
        size_t rlen = 0;
        while (t > 0) {
            rev[rlen++] = (uint8_t)(t % 10);
            t /= 10;
        }
        for (size_t i = 0; i < rlen; ++i) {
            digits[i] = rev[rlen - 1 - i];
        }
        len = rlen;
    }

    /* Try deleting each digit. */
    for (size_t pos = 0; pos < len; ++pos) {
        seq_t num = 0;
        for (size_t j = 0; j < len; ++j) {
            if (j == pos) continue;
            num = num * 10 + digits[j];
        }
        if (is_prime_big(num)) return true;
    }
    return false;
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t last_checked = 0;

    if (n <= cache_len) return cache[n - 1];

    /* Ensure enough storage. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Search for new qualifying numbers. */
    for (idx_t cand = last_checked + 1; cache_len < n; ++cand) {
        if (cand < 10) continue;     /* single‑digit numbers cannot be qualified */
        if (qualifies(cand)) {
            cache[cache_len++] = (seq_t)cand;
        }
        last_checked = cand;
    }

    return cache[n - 1];
}
