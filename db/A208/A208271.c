/*
 * A208271  Nonprime numbers containing a digit 1.
 * https://oeis.org/A208271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth nonprime number that contains the digit 1.
   The sequence is OEIS A208271.  It is computed by a simple sieve that
   keeps a cache of already-found terms so that any order of calls works. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_digits.h"
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

/* Check whether the decimal representation of x contains the digit 1. */
static bool contains_one(seq_t x)
{
    uint8_t buf[200];
    size_t len;
    get_digits(x, buf, &len);     /* buf stores digits 0‑9 */
    for (size_t i = 0; i < len; ++i) {
        if (buf[i] == 1)          /* digit 1 found */
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset 1: a(1) is the first term */
        return 0;

    /* Static cache: cache[0] == a(1), cache[1] == a(2), ... */
    static seq_t *cache       = NULL;   /* array of cached terms */
    static size_t cache_len   = 0;      /* number of terms cached so far */
    static size_t cache_cap   = 0;      /* current capacity of cache array */
    static seq_t next_candidate = 1;    /* next number to test */

    /* Grow cache capacity if necessary */
    if (n > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? n : cache_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;          /* allocation failure (unlikely) */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until the cache reaches the required size */
    while (cache_len < (size_t)n) {
        seq_t cur = next_candidate;
        /* a nonprime number containing the digit 1 */
        if (!is_prime_big(cur) && contains_one(cur))
            cache[cache_len++] = cur;
        next_candidate = cur + 1;
    }

    return cache[n - 1];
}
