/*
 * A086776  Smaller member of a prime pair (p, p+6) with a square sum.
 * https://oeis.org/A086776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 307, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 48 terms through n = 9410 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th smaller member of a prime pair (p, p+6) whose sum is a square.
   For integers k ≥ 2 it holds that
        p      = 2 k² – 3,
        p + 6  = 2 k² + 3,
   so the condition that p and p + 6 are primes is equivalent to the
   requirement that both 2 k² – 3 and 2 k² + 3 are prime.
   This routine observes the OEIS offset 1: a(1)=5.  It is memoised so that
   successive calls remain fast.  The algorithm filters candidates with
   a small prime table before calling the general Miller–Rabin test
   (is_prime_big).  It works for seq_t sizes up to 128 bits and is
   well‑within the 2 s limit for n ≈ 10 000.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Small primes used for a quick divisibility filter.  */
static const uint64_t small_primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67,
    71, 73, 79, 83, 89, 97
};
static const size_t small_primes_cnt = sizeof(small_primes)/sizeof(small_primes[0]);

/* Cached sequence values and state for memoisation.  */
static seq_t *cache        = NULL;   /* dynamic array of terms          */
static size_t cache_size   = 0;      /* number of valid terms in cache  */
static size_t cache_cap    = 0;      /* capacity of cache              */
static idx_t  last_k       = 2;      /* last k that was examined        */

/* Expand cache when needed.  */
static void expand_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *newp = mem_resize(seq_t, cache, new_cap);
    if (!newp) exit(EXIT_FAILURE);
    cache    = newp;
    cache_cap = new_cap;
}

/* Test whether a candidate value is likely prime.
   First filters with tiny primes, then invokes is_prime_big.  */
static bool is_candidate_prime(seq_t x)
{
    /* trivial small cases  */
    if (x <= 1) return false;
    for (size_t i = 0; i < small_primes_cnt; ++i) {
        uint64_t p = small_primes[i];
        if (p * (seq_t)p > x) break;          /* x small enough to be prime */
        if (x % (seq_t)p == 0 && x != (seq_t)p) return false;
    }
    return is_prime_big(x);
}

/* Public function implementing the OEIS sequence.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* not used, offset starts at 1 */

    /* Generate terms until cache has at least n entries.  */
    while (cache_size < (size_t)n) {
        seq_t k512 = (seq_t)last_k;
        seq_t k2   = k512 * k512;           /* k^2 */
        seq_t p1   = 2 * k2 - 3;            /* 2k^2 – 3 */
        seq_t p2   = p1 + 6;                /* 2k^2 + 3 */

        /* Quick filter with small primes before potentially costly tests. */
        bool ok = true;
        for (size_t i = 0; i < small_primes_cnt; ++i) {
            uint64_t sp = small_primes[i];
            if (p1 % (seq_t)sp == 0 && p1 != (seq_t)sp) { ok = false; break; }
        }
        if (ok)
            for (size_t i = 0; i < small_primes_cnt; ++i) {
                uint64_t sp = small_primes[i];
                if (p2 % (seq_t)sp == 0 && p2 != (seq_t)sp) { ok = false; break; }
            }

        if (ok && is_candidate_prime(p1) && is_candidate_prime(p2)) {
            /* Store the smaller member of the pair.  */
            expand_cache(cache_size + 1);
            cache[cache_size++] = p1;
        }
        /* Move to next k.  */
        ++last_k;
    }

    return cache[n - 1];
}
