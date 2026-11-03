/*
 * A087592  Primes whose successive differences are increasing power of 2:
 *          a(1) = 2, a(n+1) = a(n) + 2^k; a(n+1) prime, k minimal and greater
 *          than the index for the previous term.
 * https://oeis.org/A087592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   is_prime_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A087592: Primes whose successive differences are increasing powers of two.
 * a(1) = 2, a(2) = 3.
 * For n > 2: let p(n-1) be a(n-1) and k1 the exponent used to reach p(n-1).
 *           a(n) = p(n-1) + 2^k where k is the smallest integer > k1 such that the sum is prime.
 * The function caches previously computed terms and remembers the last exponent
 * so that any n ≥ 1 can be answered in order‑independent fashion.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/pow2.h"
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Static cache of computed terms and associated state */
    static seq_t *cache     = NULL;
    static idx_t cache_len  = 0;   /* number of terms stored */
    static idx_t cache_cap  = 0;   /* allocated capacity of cache */
    static idx_t prev_exp   = 0;   /* exponent used to obtain last stored term */

    /* Return cached value if already computed */
    if (cache_len >= n)
        return cache[n - 1];

    /* Initialise cache if this is the first call */
    if (!cache) {
        cache_cap = 16;
        cache = seq_alloc(cache_cap);

        /* a(1) = 2, a(2) = 3 */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache_len = 2;
        prev_exp  = 0;   /* exponent used to reach a(2) is 0 (2^0 = 1) */
    }

    /* Generate terms up to the requested index */
    while (cache_len < n) {
        /* Grow cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap * 2;
            cache = seq_realloc(cache, cache_len, cache_cap);
        }

        /* Find the smallest exponent k > prev_exp such that
           cache[cache_len-1] + 2^k is prime. */
        seq_t prev = cache[cache_len - 1];
        idx_t k = prev_exp + 1;
        seq_t candidate;
        for (;;) {
            candidate = prev + pow2(k);
            if (is_prime_big(candidate)) {
                cache[cache_len] = candidate;
                prev_exp = k;
                break;
            }
            ++k;
        }
        ++cache_len;
    }

    return cache[n - 1];
}
