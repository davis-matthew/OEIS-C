/*
 * A076979  Deleting exactly one digit yields a prime.
 * https://oeis.org/A076979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 152, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 7409 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that become prime when exactly one decimal digit is deleted */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
        seq_t *new_cache = mem_new(seq_t, new_cap);
        if (!new_cache) exit(EXIT_FAILURE);
        if (cache) {
            mem_copy(new_cache, cache, cache_len * sizeof(seq_t));
            mem_free(cache);
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
