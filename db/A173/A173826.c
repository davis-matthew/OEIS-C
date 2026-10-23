/*
 * A173826  Primes p such that p-+floor(Sqrt(p)) are primes.
 * https://oeis.org/A173826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p−⌊√p⌋ and p+⌊√p⌋ are primes. */
/* No closed formula – we generate the sequence by testing primes successively. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- utility: integer square root for 64‑bit values ---------- */
static inline uint64_t isqrt_uint64(uint64_t v)
{
    /* binary search for floor(sqrt(v)) */
    uint64_t low = 0, high = 1ULL << 32;           /* high^2 > 2^64-1 */
    while (low <= high) {
        uint64_t mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* ---------- rudimentary primality test (works for values that fit in 64 bits) ---------- */
static inline bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1ULL) == 0) return false;
    uint64_t limit = isqrt_uint64(n);
    for (uint64_t d = 3; d <= limit; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* ---------- wrapper that works on the generic seq_t type ---------- */
static inline bool is_prime(seq_t n)
{
    /* For most builds seq_t will fit into 64 bits for the values we test. */
    return is_prime_small((uint64_t)n);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – no definition for n=0 */

    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t len = 0;
    static size_t cap = 0;

    /* Last prime we examined (for continuation) */
    static seq_t last_checked = 3;

    /* Grow the cache until we have at least n terms */
    while (len < (size_t)n) {
        /* Find the next prime p after last_checked */
        seq_t p = last_checked;
        if (p <= 2) p = 3;
        if ((p & 1) == 0) p++;          /* make odd */

        while (!is_prime(p)) {          /* skip non‑primes */
            p += 2;
            if (p == (seq_t)0) {        /* overflow safeguard */
                /* In an unexpected overflow situation, abort */
                return 0;
            }
        }

        /* Compute floor(sqrt(p)) using 64‑bit helper */
        uint64_t p64 = (uint64_t)p;
        uint64_t s64 = isqrt_uint64(p64);
        seq_t s = (seq_t)s64;

        /* Test the two neighboring numbers */
        seq_t pm = p - s;
        seq_t pp = p + s;
        if (is_prime(pm) && is_prime(pp)) {
            /* Store the term */
            if (len == cap) {
                cap = cap ? cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = p;
        }

        /* Prepare for next candidate */
        last_checked = p + 2;
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
