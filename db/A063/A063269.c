/*
 * A063269  a(1) = 3, a(n) = concatenation of two closest factors of a(n-1)
 *          whose product equals a(n-1) or if a(n-1) is a prime then the
 *          concatenation of 1 and a(n-1).
 * https://oeis.org/A063269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3; for n>1, let x=a(n‑1).  
   If x is prime, a(n)=concat(1,x); otherwise let d be the largest
   divisor of x not exceeding sqrt(x) and m=x/d, then
   a(n)=concat(d,m). */

/* External integer types are defined by the build (seq_t, idx_t). */
#include <stdbool.h>
#include <stddef.h>

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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
    /* Undefined for n < 1, but return 0 as a safe default. */
    if (n < 1) return 0;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;      /* array of a(k) for k=1..cap */
    static size_t  cap   = 0;        /* current capacity of cache */
    static size_t  last  = 0;        /* number of terms already computed */

    /* Make sure the cache is large enough to hold n terms. */
    if (n > cap)
        ensure_cache(n, &cache, &cap);

    /* If the term is already known, just return it. */
    if (n <= last)
        return cache[n - 1];

    /* Compute missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t value;
        if (i == 1) {
            value = 3;                      /* a(1) = 3 */
        } else {
            seq_t prev = cache[i - 2];     /* a(i‑1) */

            if (is_prime_big(prev)) {
                /* Concatenate 1 and prev. */
                seq_t pow = pow10((idx_t)num_digits(prev));
                value = (seq_t)1 * pow + prev;
            } else {
                /* Find the closest factor pair (d,m). */
                seq_t d = floor_sqrt(prev);
                seq_t m = 0;
                for (; d >= 2; --d) {
                    if ((prev % d) == 0) {
                        m = prev / d;
                        break;
                    }
                }
                /* Concatenate d and m. */
                seq_t pow = pow10((idx_t)num_digits(m));
                value = d * pow + m;
            }
        }
        cache[i - 1] = value;   /* store a(i) */
    }

    last = n;
    return cache[n - 1];
}
