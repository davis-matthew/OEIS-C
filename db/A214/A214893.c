/*
 * A214893  Conway's subprime Fibonacci sequence starting with (18, 5).
 * https://oeis.org/A214893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Conway's subprime Fibonacci sequence starting with (18,5).
 *            Recurrence: a(1)=18, a(2)=5. For n≥3: t = a(n-1)+a(n-2); if t is
 *            composite, t = t / lpf(t), where lpf is the least prime factor
 * Range:     u8 n <= 12, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: standard, pass, 63 terms through n = 81 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_i32, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: find least prime factor of a composite number using a growing prime list */
static seq_t _lpf(seq_t x) {
    /* odd check */
    if ((x & (seq_t)1) == 0) return (seq_t)2;

    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    seq_t sq = floor_sqrt(x);
    idx_t lim = (idx_t)sq;
    if (lim > primes_limit) {
        free(primes);
        build_primes_upto(lim, &primes, &primes_cnt);
        primes_limit = lim;
    }
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t pt = (seq_t)p;
        if (x % pt == 0) return pt;
    }
    /* Should not happen for composite x */
    return x;
}

/* Main function: compute the sequence */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined but safe guard */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (cache_size == 0) {
        /* allocate initial space for first two terms */
        cache = mem_new(seq_t, 2);
        cache[0] = (seq_t)18;
        cache[1] = (seq_t)5;
        cache_size = 2;
    }

    if ((idx_t)n > cache_size) {
        size_t new_size = (size_t)n;
        cache = mem_resize(seq_t, cache, new_size);

        for (size_t i = cache_size; i < new_size; ++i) {
            seq_t tmp = cache[i - 1] + cache[i - 2];
            if (!isPrimeGeneric(tmp)) {
                seq_t lpf = _lpf(tmp);
                tmp = tmp / lpf;
            }
            cache[i] = tmp;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n - 1];
}
