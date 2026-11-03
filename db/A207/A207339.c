/*
 * A207339  Triangular numbers T from A000217 such that (4*T+1)/5 is prime.
 * https://oeis.org/A207339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 975 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207339: Triangular numbers T such that (4*T+1)/5 is prime
   The function returns the n‑th term of that sequence (n ≥ 1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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
    /* static memoisation table for found triangular numbers */
    static seq_t *cache  = NULL;   /* array of triangular numbers */
    static size_t  used  = 0;      /* number of elements stored */
    static size_t  cap   = 0;      /* current capacity of cache */
    static idx_t   next_k = 1;     /* next index k to test */

    /* extend cache until we have at least n terms */
    while (used < n)
    {
        /* compute T_k = k*(k+1)/2 using seq_t arithmetic */
        seq_t kk  = (seq_t)next_k;
        seq_t T   = (kk * (kk + 1)) / 2;

        /* compute x = 4*T+1 */
        seq_t x   = (T * 4) + 1;

        /* check divisibility by 5; if true, evaluate the prime candidate */
        if ((x % 5) == 0)
        {
            seq_t p = x / 5;
            if (is_prime_big(p))
            {
                /* grow cache if necessary */
                if (used == cap)
                {
                    size_t newcap = (cap == 0) ? 16 : cap * 2;
                    if (cap == 0)
                        cache = seq_alloc(newcap);
                    else
                        cache = seq_realloc(cache, cap, newcap);
                    cap = newcap;
                }
                cache[used++] = T;
            }
        }
        ++next_k;
    }
    /* return the n‑th triangular number (offset 1) */
    return cache[n - 1];
}
