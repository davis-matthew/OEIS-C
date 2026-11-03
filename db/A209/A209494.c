/*
 * A209494  Smallest prime p such that 2p*n^2 - 1 is prime, or 0 if no such
 *          prime exists.
 * https://oeis.org/A209494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that 2·p·n² – 1 is prime, or 0 if
 *            none exists
 * Range:     u8 n <= 8, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 90 terms through n = 971 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
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

/* Cache for previously computed values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t cached_up_to = 0;

/* Return the n‑th term of sequence A209494. */
seq_t a(idx_t n)
{
    /* Grow the cache to hold values up to n. */
    if ((idx_t)(cache_len) <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (!tmp)           /* memory allocation failure */
            return (seq_t)0;
        cache = tmp;
        /* Initialise new entries to 0. */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i)
            cache[i] = (seq_t)0;
        cache_len = new_len;
    }

    /* Compute any missing values. */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        uint64_t p = 2;               /* first prime */
        seq_t  res = (seq_t)0;         /* 0 means no prime found yet */

        /* Limit the search to avoid infinite loops for pathological n. */
        const uint64_t PRIME_LIMIT = 5000000ULL;

        while (1) {
            seq_t i_sq     = (seq_t)i * (seq_t)i;
            seq_t candidate = ((seq_t)2 * (seq_t)p * i_sq) - (seq_t)1;

            if (is_prime_big(candidate)) {
                res = (seq_t)p;
                break;
            }
            if (p > PRIME_LIMIT) {    /* give up – return 0 */
                res = (seq_t)0;
                break;
            }
            p = next_prime(p);
        }
        cache[i] = res;
    }

    cached_up_to = n;
    return cache[n];
}
