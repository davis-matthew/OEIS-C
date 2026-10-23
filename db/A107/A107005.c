/*
 * A107005  Primes of the form 4x^2+4xy+7y^2, with x and y nonnegative.
 * https://oeis.org/A107005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107005 – primes of the form 4x²+4xy+7y² (x,y ≥ 0) */

/* The sequence is the ascending list of primes of the form
   4*x*x + 4*x*y + 7*y*y.  The value a(n) is the n‑th prime in increasing
   order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"

#if defined(__cplusplus)
#include <vector>
#endif

/* Integer square root for 0 ≤ x < 2⁶⁴ (uint64_t).  The result fits in uint64_t. */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = 0;
    for (int i = 31; i >= 0; --i) {
        uint64_t t = r | (1ULL << i);
        if ((__uint128_t)t * t <= x)
            r = t;
    }
    return r;
}

/* ---------- Storage of primes ---------- */
#if defined(__cplusplus)
static std::vector<seq_t> primes;
#else
static seq_t *primes = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;
#endif

static void add_prime(seq_t v)
{
#if defined(__cplusplus)
    /* Find insertion point keeping the vector sorted. */
    size_t lo = 0, hi = primes.size();
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < primes.size() && primes[lo] == v)
        return;
    primes.insert(primes.begin() + lo, v);
#else
    /* Binary search in the array. */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < prime_count && primes[lo] == v)
        return;

    /* Ensure capacity. */
    if (prime_count == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);
        primes = tmp;
        prime_cap = new_cap;
    }

    /* Shift elements to make room. */
    for (size_t i = prime_count; i > lo; --i)
        primes[i] = primes[i - 1];
    primes[lo] = v;
    prime_count++;
#endif
}

/* ---------- Storage of seen values ---------- */
static bool *seen = NULL;
static size_t seen_cap = 0;

/* ---------- Helper to grow the seen array ---------- */
static void ensure_seen_cap(size_t new_cap)
{
    if (new_cap <= seen_cap)
        return;
    bool *tmp = NULL;
#if defined(__cplusplus)
    tmp = new bool[new_cap];
    memset(tmp, 0, new_cap * sizeof(bool));
    if (seen) {
        memcpy(tmp, seen, seen_cap * sizeof(bool));
        delete[] seen;
    }
#else
    tmp = (bool *)realloc(seen, new_cap * sizeof(bool));
    memset(tmp + seen_cap, 0, (new_cap - seen_cap) * sizeof(bool));
    if (!tmp)
        exit(1);
#endif
    seen = tmp;
    seen_cap = new_cap;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* out of range – not used by tests */

    /* Fast return if we already have enough primes. */
#if defined(__cplusplus)
    if (primes.size() >= (size_t)n)
        return primes[(size_t)n - 1];
#else
    if (prime_count >= (size_t)n)
        return primes[(size_t)n - 1];
#endif

    /* The largest number we have processed so far. */
    static idx_t last_limit = 0;

    /* Generate values until we have at least n primes. */
    while (true) {
        idx_t new_limit = (last_limit == 0) ? 20ULL : last_limit * 2ULL;

        /* Grow the seen array to cover values up to new_limit. */
        ensure_seen_cap((size_t)new_limit + 1);

        /* Enumerate all (x,y) with 4x²+4xy+7y² ≤ new_limit. */
        uint64_t max_y = isqrt_uint64(new_limit / 7ULL);
        for (uint64_t y = 0; y <= max_y; ++y) {
            uint64_t x = 0;
            while (1) {
                __uint128_t v128 =
                    (__uint128_t)4 * x * x
                  + (__uint128_t)4 * x * y
                  + (__uint128_t)7 * y * y;
                if (v128 > new_limit)
                    break;
                uint64_t v = (uint64_t)v128;
                if (!seen[v]) {
                    seen[v] = true;
                    if (v >= 7 && is_prime(v))
                        add_prime((seq_t)v);
                }
                ++x;
            }
        }

        last_limit = new_limit;

        /* Check if we now have enough primes. */
#if defined(__cplusplus)
        if (primes.size() >= (size_t)n)
            return primes[(size_t)n - 1];
#else
        if (prime_count >= (size_t)n)
            return primes[(size_t)n - 1];
#endif
    }
}
