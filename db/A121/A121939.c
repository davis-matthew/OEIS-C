/*
 * A121939  Palindromic numbers that contain the sum of their digits as a
 *          substring.
 * https://oeis.org/A121939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 42, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, digit_sum, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromic numbers that contain the sum of their digits as a substring.
   a(n) is the n-th such number, with a(1)=1. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e); */
#include "../utils/digit_sum.h"       /* seq_t digit_sum(seq_t v); */
#include "../utils/digit_len.h"       /* size_t digit_len(seq_t v); */

/* Build a palindrome of total length 'len' from the left half 'half'.
   For odd 'len', the middle digit is the last digit of 'half'.
   All operations are performed using seq_t. */
static inline seq_t build_pal(seq_t half, idx_t len)
{
    seq_t pal = half;
    seq_t tmp = (len & 1) ? half / 10 : half; /* skip middle digit for odd len */
    while (tmp > 0) {
        pal = pal * 10 + (tmp % 10);
        tmp /= 10;
    }
    return pal;
}

/* Check whether the decimal sum of digits of 'v' occurs as a contiguous
   substring of the decimal representation of 'v'. */
static inline bool contains_sum_subseq(seq_t v)
{
    /* sum of digits as seq_t */
    seq_t sum = digit_sum(v);

    /* get digit lengths */
    size_t vlen = digit_len(v);
    size_t slen = digit_len(sum);
    if (slen == 0) return false;

    /* extract digits of v (reversed) */
    uint8_t *vdig = (uint8_t*)malloc(vlen);
    seq_t tmp = v;
    for (size_t i = 0; i < vlen; ++i) {
        vdig[i] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    /* extract digits of sum (reversed) */
    uint8_t *smdig = (uint8_t*)malloc(slen);
    for (size_t i = 0; i < slen; ++i) {
        smdig[i] = (uint8_t)(sum % 10);
        sum /= 10;
    }

    /* reverse to most significant first */
    for (size_t i = 0; i < vlen / 2; ++i) {
        uint8_t t = vdig[i]; vdig[i] = vdig[vlen - 1 - i]; vdig[vlen - 1 - i] = t;
    }
    for (size_t i = 0; i < slen / 2; ++i) {
        uint8_t t = smdig[i]; smdig[i] = smdig[slen - 1 - i]; smdig[slen - 1 - i] = t;
    }

    /* search for smdig in vdig */
    bool found = false;
    if (slen <= vlen) {
        for (size_t i = 0; i + slen <= vlen; ++i) {
            bool ok = true;
            for (size_t j = 0; j < slen; ++j) {
                if (vdig[i + j] != smdig[j]) { ok = false; break; }
            }
            if (ok) { found = true; break; }
        }
    }

    free(vdig);
    free(smdig);
    return found;
}

/* ---------- caching infrastructure ---------- */
static seq_t *cache = NULL;      /* dynamic array of known terms */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity */

/* add a value to the cache, expanding if necessary */
static void cache_add(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_vec = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_vec) abort(); /* out of memory */
        cache = new_vec;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- palindrome generation state ---------- */
static idx_t cur_len = 0;      /* current length of palindromes being generated */
static seq_t cur_prefix = 0;   /* current left half being considered */
static seq_t cur_end = 0;      /* largest left half for cur_len */
static bool initialized = false;

/* update the generation state to start a new length */
static void advance_length(void)
{
    ++cur_len;
    int prefix_len = (cur_len + 1) / 2;
    cur_prefix = pow10((idx_t)(prefix_len - 1));
    cur_end    = pow10((idx_t)prefix_len) - 1;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* lazy initialization */
    if (!initialized) {
        cur_len = 1;
        int prefix_len = (cur_len + 1) / 2;
        cur_prefix = pow10((idx_t)(prefix_len - 1));
        cur_end    = pow10((idx_t)prefix_len) - 1;
        initialized = true;
    }
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort(); /* out of memory */
    }

    while (cache_len < n) {
        seq_t pal = build_pal(cur_prefix, cur_len);
        if (contains_sum_subseq(pal))
            cache_add(pal);

        /* move to next prefix */
        cur_prefix = cur_prefix + 1;
        if (cur_prefix > cur_end) {
            advance_length();
        }
    }
    return cache[n - 1];
}
