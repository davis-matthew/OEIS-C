/*
 * A178050  n=x^2+17, n and n+2 are prime.
 * https://oeis.org/A178050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: numbers n = x^2 + 17 such that n and n+2 are prime (twin primes). */
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/is_prime_u64.h"

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

seq_t a(idx_t n)
{
    /* static cache for previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t count    = 0;   /* number of terms stored in cache */
    static idx_t next_x   = 0;   /* next x to test (x >= 0) */

    if (n == 0)   /* offset starts at 1, behaviour for n==0 is undefined */
        return 0;

    /* if the requested term is already cached, return it immediately */
    if (n <= count)
        return cache[n - 1];

    /* generate terms until we reach the nth one */
    for (;;)
    {
        seq_t candidate = (seq_t)next_x * (seq_t)next_x + (seq_t)17;

        /* check whether candidate and candidate+2 are prime */
        if (is_prime_big(candidate) && is_prime_big(candidate + (seq_t)2))
        {
            /* enlarge cache if necessary */
            if (count == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                seq_t *tmp     = seq_realloc(cache, cache_cap, new_cap);
                cache          = tmp;
                cache_cap      = new_cap;
            }
            cache[count] = candidate;
            ++count;
        }
        ++next_x;
        if (count >= n)
            break;
    }
    return cache[n - 1];
}
