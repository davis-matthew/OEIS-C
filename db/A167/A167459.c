/*
 * A167459  Composite numbers in A166504, i.e., whose decimal expansion can be
 *          split up into prime numbers, with leading zeros allowed.
 * https://oeis.org/A167459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n-th composite integer whose decimal expansion can be
 * partitioned into prime numbers, allowing leading zeros in the prime parts.
 * The sequence includes only composite members of A166504.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
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

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* DP test for whether a number can be split into prime substrings.
 * Leading zeros in parts are treated as the prime value of the parts. */
static bool can_partition(seq_t num)
{
    /* Convert number to its decimal string representation */
    char buf[128];
    size_t len;
    seq_to_str(num, buf, &len);

    /* dp[i] == true if prefix of length i can be partitioned successfully */
    bool dp[128] = {0};
    dp[0] = true;

    for (size_t i = 1; i <= len; ++i) {
        bool ok = false;
        for (size_t j = 0; j < i && !ok; ++j) {
            if (!dp[j]) continue;

            /* Extract substring j..i-1 */
            size_t sublen = i - j;
            if (sublen >= sizeof(buf)) continue; /* safety; should not happen */
            char sub[128];
            memcpy(sub, buf + j, sublen);
            sub[sublen] = '\0';

            seq_t val = seq_from_str(sub);
            if (val >= (seq_t)2 && is_prime_big(val))
                ok = true;
        }
        dp[i] = ok;
    }
    return dp[len];
}

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 4;   /* 2 and 3 are primes, 4 is first composite */

    if (n == 0) return (seq_t)0; /* undefined for n=0, but safe default */

    /* Generate terms until we reach the requested index */
    while (cache_len < n) {
        idx_t cand = next_candidate;

        /* Find next qualifying composite number */
        while (1) {
            seq_t cand_seq = (seq_t)cand;
            if (!is_prime_big(cand_seq) && can_partition(cand_seq))
                break;
            ++cand;
        }

        /* Grow cache if needed */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)cand;
        next_candidate = cand + 1;
    }

    return cache[n - 1];
}
