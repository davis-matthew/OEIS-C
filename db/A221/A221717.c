/*
 * A221717  Non-cuban primes.
 * https://oeis.org/A221717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑cuban primes: primes that are NOT of the form 3k²-3k+1
   (the Cuban primes).  Computed by incremental trial from the
   prime’s start (2) and skipping values that are cubic primes. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- basic helpers ---------- */

/* Integer square‑root, floor, of a 64‑bit unsigned integer. */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x, mid, sq;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            if (mid == hi) break;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Test if a 64‑bit number p can be written as 3k²-3k+1. */
static bool is_cuban_prime_u64(uint64_t p)
{
    /* 12p - 3 must be a perfect square and sqrt(12p-3) ≡ 3 (mod 6). */
    uint64_t v = 12 * p - 3;
    uint64_t d = isqrt_u64(v);
    return (d * d == v) && ((d + 3) % 6 == 0);
}

/* ---------- prime test ---------- */

/* The utility is_prime(idx_t x) is available in "../utils/is_prime.h". */
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t x); */

/* ---------- caching mechanism ---------- */

static seq_t *cache = NULL;      /* array of the non‑cuban primes found so far */
static size_t cache_len = 0;     /* number of stored primes */
static size_t cache_cap = 0;     /* current capacity of the array */
static uint64_t candidate = 2;   /* current number to test for primality */

/* Ensure that the cache can hold at least 'needed' entries. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);      /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- main function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* Fast path for when we already have the value cached. */
    if (cache_len >= n) return cache[n - 1];

    /* Special handling for candidate == 2, then move to 3 and then odd numbers only. */
    if (candidate == 2) {
        /* 2 is a prime but does it need to be excluded?  2 = 3k²-3k+1 gives k=1 -> 1
           which is not valid, so 2 is kept. */
        if (!is_cuban_prime_u64(2)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)2;
        }
        candidate = 3;  /* next candidate will be odd */
    }

    /* Main loop: keep finding primes until we have at least n of them. */
    while (cache_len < n) {
        /* Test primality of current candidate. */
        if (is_prime((idx_t)candidate)) {
            /* Keep the prime unless it is a Cuban prime. */
            if (!is_cuban_prime_u64(candidate)) {
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = (seq_t)candidate;
            }
        }
        /* Advance to next odd number. */
        candidate += 2;
    }

    return cache[n - 1];
}
