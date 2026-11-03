/*
 * A109208  Palindromic primes p such that digit sum of p is a substring.
 * https://oeis.org/A109208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 54 (2026-11-03)
 * Helpers:   digit_sum_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A109208: palindromic primes whose digit sum is a substring
   Formula: a(n) = the n-th palindromic prime p such that
              the decimal digit sum of p occurs as a contiguous
              substring of the decimal representation of p */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>

#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_sum_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Convert a seq_t value to a decimal string.  The buffer must be
   large enough to hold the result.  */
static void seq_to_dec_string(seq_t val, char *buf, size_t buflen)
{
    seq_t v = val;
    if (v == 0) {
        if (buflen >= 2) {
            buf[0] = '0';
            buf[1] = '\0';
        }
        return;
    }
    size_t pos = 0;
    /* Extract digits in reverse order. */
    while (v != 0 && pos + 1 < buflen) {
        seq_t d = v % 10;
        v = v / 10;
        buf[pos++] = (char)('0' + (int)d);
    }
    /* Null‑terminate and reverse the string. */
    if (pos >= buflen) pos = buflen - 1;
    buf[pos] = '\0';
    for (size_t i = 0; i < pos / 2; ++i) {
        char tmp = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = tmp;
    }
}

/* Build a palindrome from its left half.
   For even length we mirror all digits of half.
   For odd length we skip the middle digit of half when mirroring. */
static inline uint64_t make_pal(uint64_t half, int len) {
    uint64_t pal = half;
    uint64_t x   = half;
    if (len & 1) x /= 10U;          /* drop middle digit when odd */
    while (x > 0) {
        pal = pal * 10U + (x % 10U);
        x  /= 10U;
    }
    return pal;
}

/* ----- helpers ----------------------------------------------------- */

static inline uint64_t pow10_uint64(int exp) {
    uint64_t r = 1;
    while (exp--) r *= 10U;
    return r;
}

/* Test whether the decimal digit sum of p appears as a substring in p. */
static bool has_sum_substring(uint64_t p) {
    uint64_t sum = digit_sum_u64(p);
    char pbuf[64];
    char sbuf[32];
    seq_to_dec_string((seq_t)p, pbuf, sizeof(pbuf));
    /* convert sum to decimal string */
    snprintf(sbuf, sizeof(sbuf), "%" PRIu64, sum);
    return strstr(pbuf, sbuf) != NULL;
}

/* ----- cache and generation ---------------------------------------- */

/* Static state for memoisation */
static seq_t *cache          = NULL;   /* array of terms */
static size_t cache_size     = 0;      /* number of cached terms */
static size_t cache_capacity = 0;

/* Current palindrome generation state */
static int   cur_len  = 1;            /* current palindrome length */
static uint64_t cur_half_start = 1;   /* first half for current length */
static uint64_t cur_half_end   = 9;   /* last half for current length */
static uint64_t cur_half   = 1;       /* next half to try */

/* Ensure the cache contains at least `target` terms. */
static void generate_until(size_t target) {
    /* Allocate cache lazily */
    if (cache == NULL) {
        cache_capacity = 1024;
        cache = mem_new(seq_t, cache_capacity);
    }

    /* Generate terms until we have enough */
    while (cache_size < target) {
        uint64_t pal = make_pal(cur_half, cur_len);
        if (is_prime_u64(pal) && has_sum_substring(pal)) {
            /* Grow cache if necessary */
            if (cache_size >= cache_capacity) {
                cache_capacity *= 2;
                cache = mem_resize(seq_t, cache, cache_capacity);
            }
            cache[cache_size++] = (seq_t)pal;
        }

        /* advance half */
        ++cur_half;
        if (cur_half > cur_half_end) {
            /* move to next length */
            ++cur_len;
            int half_len = (cur_len + 1) / 2;
            cur_half_start = pow10_uint64(half_len - 1);
            cur_half_end   = pow10_uint64(half_len) - 1;
            cur_half = cur_half_start;
        }
    }
}

/* ----- main sequence function ------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* undefined for n=0 */
    if ((size_t)n > cache_size)
        generate_until((size_t)n);
    return cache[n - 1];
}
