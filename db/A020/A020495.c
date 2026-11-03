/*
 * A020495  Neither square nor square + prime.
 * https://oeis.org/A020495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are neither a perfect square nor representable as a
   square plus a prime.  The sequence is constructed by searching
   the positive integers, discarding squares, discarding primes
   (since p = 0^2 + p), and discarding any n for which n - t^2
   is prime for some integer t ≥ 1.  The remaining numbers form
   the sequence A020495. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Static cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
/* Next number to test */
static seq_t next_candidate = 2;

seq_t a(idx_t n)
{
    /* Ensure we have computed at least n terms */
    while (cache_len < n) {
        seq_t cand = next_candidate++;          /* candidate under test */

        /* Skip perfect squares */
        seq_t sqrt_cand = isqrt_seq(cand);
        if (sqrt_cand * sqrt_cand == cand)
            continue;

        /* Skip primes (p = 0^2 + p) */
        if (is_prime_big(cand))
            continue;

        /* Check if cand can be expressed as t^2 + prime for some t >= 1 */
        bool ok = true;
        /* Only t values whose square is at most cand-2 can give prime diff >= 2 */
        seq_t limit = cand > 2 ? cand - 2 : 0;
        seq_t sqrt_limit = isqrt_seq(limit);

        for (idx_t t = 1; t <= (idx_t)sqrt_limit; ++t) {
            seq_t diff = cand - (seq_t)t * t;
            if (diff >= 2 && is_prime_big(diff)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            append_candidate(&cache, &cache_len, &cache_cap, cand);
        }
    }

    return cache[n - 1];
}
