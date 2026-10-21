/*
 * A062888  Smallest palindromic multiple of n-th prime.
 * https://oeis.org/A062888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   build_pal_with_rem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest palindromic multiple of the n‑th prime */
/* The algorithm enumerates palindromes of increasing length and
   checks divisibility by the n‑th prime.  Primes are generated
   lazily by trial division.  All computations fit in the
   provided integer type `seq_t`. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "../utils/build_pal_with_rem.h"

/* ------- Utility: power of ten for 64‑bit integers ------- */
static uint64_t pow10_uint64(int e)
{
    uint64_t r = 1;
    while (e--) r *= 10ull;
    return r;
}

/* ------- Prime cache ------- */
#define PRIME_CACHE_CAP 200000   /* > 10 000th prime (≈104 729) */

static uint32_t prime_cache[PRIME_CACHE_CAP];
static size_t   prime_cache_cnt = 0;
static uint32_t prime_cache_next = 2;

/* Ensure the cache contains at least `cnt` primes.  Generates
   new primes by simple trial division using the primes already in
   the cache. */
static void ensure_primes_upto_count(size_t cnt)
{
    if (prime_cache_cnt >= cnt) return;

    if (prime_cache_cnt == 0) {
        prime_cache[0] = 2;
        prime_cache_cnt = 1;
        prime_cache_next = 3;
    }

    while (prime_cache_cnt < cnt) {
        uint32_t cand = prime_cache_next;
        uint32_t lim = (uint32_t)(sqrt((double)cand) + 1.0);
        bool ok = true;
        for (size_t i = 0; i < prime_cache_cnt && prime_cache[i] <= lim; ++i) {
            if (cand % prime_cache[i] == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            prime_cache[prime_cache_cnt++] = cand;
        }
        cand += 2;
        prime_cache_next = cand;
    }
}

/* Return the n‑th prime (1‑based). */
static uint32_t nth_prime(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_primes_upto_count(idx);
    return prime_cache[idx - 1];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    uint32_t p = nth_prime(n);          /* n‑th prime */

    /* Check single‑digit palindromes first. */
    for (uint8_t d = 1; d <= 9; ++d) {
        if (d % p == 0) return d;
    }

    /* Iterate over palindrome lengths, starting from 2. */
    for (int len = 2; ; ++len) {
        if (len % 2 == 0) { /* even length */
            int half_len = len / 2;
            uint64_t start = pow10_uint64(half_len - 1);
            uint64_t end   = pow10_uint64(half_len) - 1u;
            for (uint64_t half = start; half <= end; ++half) {
                uint64_t rem;
                seq_t pal = build_pal_with_rem(p, half, half_len, false, 0, &rem);
                if (rem == 0) return pal;
            }
        } else { /* odd length */
            int half_len = len / 2;
            uint64_t start = pow10_uint64(half_len - 1);
            uint64_t end   = pow10_uint64(half_len) - 1u;
            for (uint64_t half = start; half <= end; ++half) {
                for (uint8_t mid = 0; mid <= 9; ++mid) {
                    uint64_t rem;
                    seq_t pal = build_pal_with_rem(p, half, half_len, true, mid, &rem);
                    if (rem == 0) return pal;
                }
            }
        }
    }
}
