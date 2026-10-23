/*
 * A032573  Numbers k whose decimal representation, read as a base-21 value
 *          and divided by k, yields an integer.
 * https://oeis.org/A032573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 29, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth positive integer k such that its decimal
   representation, read as a base‑21 number, is divisible by k. */

/* The implementation scans integers k in increasing order,
   keeps a cache of found terms and returns the requested n‑th term.
   It works for both C (any unsigned integer seq_t) and C++ (an
   arbitrary‑precision integer class, or any unsigned integer). */

#ifdef __cplusplus
#include <vector>
#endif
#include <stddef.h>
#include <stdint.h>

/* Multiply a by 21 modulo mod, using 128‑bit arithmetic if available. */
static inline unsigned long long mulmod21(unsigned long long a, unsigned long long mod)
{
#if defined(__SIZEOF_INT128__)
    return (unsigned long long)(((__uint128_t)a * 21ULL) % mod);
#else
    /* Fallback: repeated doubling without overflow. */
    unsigned long long res = 0;
    unsigned long long base = a % mod;
    unsigned long long exp = 21;
    while (exp)
    {
        if (exp & 1)
        {
            res += base;
            if (res >= mod) res -= mod;
        }
        /* Double base: base = (base * 2) % mod, avoiding overflow. */
        if (base >= mod - base)
            base = base + base - mod;
        else
            base = base + base;
        exp >>= 1;
    }
    return res;
#endif
}

#if defined(__cplusplus)
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; return 0 for safety */

    static std::vector<seq_t> cache;
    static unsigned long long cur_candidate = 1ULL;   /* next integer to test */

    while (cache.size() < n)
    {
        unsigned long long cand = cur_candidate;
        unsigned long long mod = 0;
        unsigned long long temp = cand;   /* for digit extraction */
        unsigned char digits[20];
        int len = 0;

        /* Grab decimal digits (least‑significant first). */
        if (temp == 0)
        {
            digits[len++] = 0;
        }
        else
        {
            while (temp > 0) {
                digits[len++] = (unsigned char)(temp % 10);
                temp /= 10;
            }
        }

        /* Process digits from most‑significant to least. */
        for (int i = len - 1; i >= 0; --i)
        {
            mod = mulmod21(mod, cand);
            mod += digits[i];
            if (mod >= cand) mod -= cand;
        }

        if (mod == 0)
            cache.push_back((seq_t)cand);

        ++cur_candidate;
    }

    return cache[n - 1];
}
#else  /* plain C implementation */
#ifndef NULL
#define NULL ((void*)0)
#endif

static inline unsigned long long mulmod21_u64(unsigned long long a, unsigned long long mod)
{
#if defined(__SIZEOF_INT128__)
    return (unsigned long long)(((__uint128_t)a * 21ULL) % mod);
#else
    unsigned long long res = 0;
    unsigned long long base = a % mod;
    unsigned long long exp = 21;
    while (exp)
    {
        if (exp & 1)
        {
            res += base;
            if (res >= mod) res -= mod;
        }
        /* Double base without overflow */
        if (base >= mod - base)
            base = base + base - mod;
        else
            base = base + base;
        exp >>= 1;
    }
    return res;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; return 0 for safety */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static unsigned long long cur_candidate = 1ULL;   /* next integer to test */

    /* Grow cache until we have at least n terms. */
    while (cache_size < n)
    {
        unsigned long long cand = cur_candidate;
        unsigned long long mod = 0;
        unsigned long long tmp = cand;   /* for digit extraction */
        unsigned char digits[20];
        int len = 0;

        if (tmp == 0)
        {
            digits[len++] = 0;
        }
        else
        {
            while (tmp > 0) {
                digits[len++] = (unsigned char)(tmp % 10);
                tmp /= 10;
            }
        }

        for (int i = len - 1; i >= 0; --i)
        {
            mod = mulmod21_u64(mod, cand);
            mod += digits[i];
            if (mod >= cand) mod -= cand;
        }

        if (mod == 0)
        {
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp_p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp_p) abort();          /* allocation failure */
                cache = tmp_p;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)cand;
        }
        ++cur_candidate;
    }

    return cache[n - 1];
}
#endif
