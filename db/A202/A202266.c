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
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
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
    seq_t *new_mem = mem_resize(seq_t, cache, new_cap);
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
