/*
 * A139033  a(1)=1; at n>=2, a(n) = least square > a(n-1) such that sum
 *          a(1)+...+a(n) is a prime number.
 * https://oeis.org/A139033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 215, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 53 terms through n = 1698 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A139033:  a(1)=1; for n≥2, a(n)=least square > a(n‑1)  
   such that a(1)+…+a(n) is prime.  This is equivalent to  
   a(n)=A137326(n)^2.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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
    static seq_t *vals   = NULL;          /* sequence values, 1‑based → vals[0]=a(1) */
    static size_t cap    = 0;             /* malloced capacity */
    static seq_t running_sum = 0;         /* Σ_{k=1}^{i-1} a(k) for current i */
    static idx_t next_compute = 1;        /* next index to compute (1‑based) */

    /* already computed? */
    if (n < next_compute)
        return vals[n - 1];

    /* enlarge storage if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        if (cap == 0)
            vals = seq_alloc(new_cap);
        else
            vals = seq_realloc(vals, cap, new_cap);
        if (!vals) abort();           /* out of memory */
        cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = next_compute; i <= n; ++i) {
        seq_t val;
        if (i == 1) {                 /* first term */
            val = (seq_t)1;
            running_sum = val;
        } else {
            seq_t prev      = vals[i - 2];   /* a(i-1) */
            seq_t prev_sum  = running_sum;   /* Σ_{k=1}^{i-1} a(k) */
            seq_t k         = floor_sqrt(prev) + 1;   /* smallest integer with k² > prev */
            while (1) {
                seq_t cand      = k * k;               /* candidate square */
                seq_t new_sum   = prev_sum + cand;     /* cumulative sum if cand chosen */
                if (isPrimeGeneric(new_sum)) {
                    val = cand;
                    running_sum = new_sum;
                    break;
                }
                ++k;
            }
        }
        vals[i - 1] = val;
    }

    next_compute = n + 1;
    return vals[n - 1];
}
