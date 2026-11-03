/*
 * A093932  For n > 0, a(n+1) is the least prime not already used such that
 *          abs(a(n+1)-a(n))/2n is prime.
 * https://oeis.org/A093932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, less
 * Formula:   a(1) = 3; for n > 1, a(n) is the least prime not already used
 *            such that abs(a(n) – a(n‑1)) / (2·(n‑1)) is prime
 * Range:     u8 n <= 19, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Validated: full, pass, 110 terms through n = 110 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence stops at a(110) = 5; for n > 110 the function returns 0. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Maximum term index in the sequence. */
#define MAX_TERMS 110

/* Cache of computed terms: cache[0] = a(1), cache[1] = a(2), … */
static seq_t cache[MAX_TERMS];
static size_t cache_len = 0;

/* Test whether p is already in the cache. */
static bool seen_prime(seq_t p)
{
    for (size_t i = 0; i < cache_len; ++i)
        if (cache[i] == p)
            return true;
    return false;
}

/* Return a(n).  For n == 0 return 0.  For n > MAX_TERMS return 0. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1 */
        return 0;

    if (n <= cache_len)
        return cache[n - 1];

    if (n > MAX_TERMS)   /* the sequence is finite. */
        return 0;

    /* Initialize first term if starting from scratch. */
    if (cache_len == 0) {
        cache[0] = (seq_t)3;   /* a(1) */
        cache_len = 1;
    }

    /* Generate terms up to the requested index. */
    for (; cache_len < n; ++cache_len) {
        seq_t prev = cache[cache_len - 1];   /* a(n‑1) */
        idx_t  prev_index = cache_len;       /* n‑1 in 1‑based indexing */

        seq_t divisor = 2 * (seq_t)prev_index;   /* 2·(n‑1) as seq_t */

        /* Search for the least unused prime satisfying the condition. */
        for (seq_t cand = (seq_t)2; ; ++cand) {
            if (!is_prime_big(cand))
                continue;
            if (seen_prime(cand))
                continue;

            seq_t diff = (cand > prev) ? (cand - prev) : (prev - cand);
            if (diff % divisor != 0)
                continue;

            seq_t q = diff / divisor;
            if (!is_prime_big(q))
                continue;

            cache[cache_len] = cand;   /* store a(n) */
            break;
        }
    }

    return cache[n - 1];
}
