/*
 * A124665  Numbers that cannot be either prefixed or followed by one digit to
 *          form a prime.
 * https://oeis.org/A124665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 161, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9372 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124665 – Numbers that cannot be either prefixed or followed
   by one digit to form a prime. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include <limits.h>
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Memoised cache. */
static seq_t *cache = NULL;     /* array of found terms   */
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* current capacity       */

/* Grow the cache array to hold at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap * 2;
    if (new_cap == 0) new_cap = 8;
    while (new_cap < needed) new_cap <<= 1;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

/* Return true if x satisfies the definition of A124665. */
static bool is_bad(seq_t x)
{
    /* x must not be prime. */
    if (isPrimeGeneric(x)) return false;

    /* Digit count of x. */
    seq_t tmp = x;
    int len = 0;
    do { len++; tmp /= 10; } while (tmp > 0);

    /* 10^len (used for prefix construction). */
    seq_t pow10k = pow10((idx_t)len);

    for (int d = 0; d <= 9; ++d)
    {
        /* Prefix: d*10^len + x.   For d==0 we get x itself. */
        seq_t pref = (seq_t)d * pow10k + x;
        if (isPrimeGeneric(pref)) return false;

        /* Suffix: x*10 + d. */
        seq_t suff = x * 10 + d;
        if (isPrimeGeneric(suff)) return false;
    }
    return true;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for 0 */

    /* Ensure cache covers at least n terms. */
    while (cache_size < n)
    {
        /* Search for the next term. */
        static seq_t cur = 1;
        do { cur++; } while (!is_bad(cur));

        grow_cache(cache_size + 1);
        cache[cache_size++] = cur;
    }
    return cache[n - 1];
}
