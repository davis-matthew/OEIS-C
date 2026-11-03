/*
 * A086762  A piecewise recurrence relation with a(2)=7 and for n>=2: if a(n)
 *          is prime, not 31, a(n+1) = A000265(3*a(n)+1); if a(n) is odd
 *          composite, not 1, a(n+1) = A000265(a(n)+1); if a(n) is even,
 *          a(n+1) = A000265(a(n)); if a(n) is 1 or 31, find the number S(n)
 *          of occurrences of 1 and 31 among a(2),a(3),....
 * https://oeis.org/A086762
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a piecewise recurrence with a(2)=7 and for n>=2 if a(n) is
 *            prime, not 31: a(n+1)=A000265(3*a(n)+1); if a(n) is odd
 *            composite, not 1: a(n+1)=A000265(a(n)+1); if a(n) is even:
 *            a(n+1)=A000265(a(n)); if a(n) is 1 or 31: let S(n) be the number
 *            of occurrences of 1 and 31 among a(2)..a(n); use 2+S(n) in...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 90 (2026-11-03)
 * Helpers:   is_prime_u64, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/odd_part.h"
#include "../utils/is_prime_u64.h"

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

/* cache with static state */
static seq_t *cache  = NULL;   /* cache[i] holds a(i+2)   */
static size_t cap    = 0;      /* size of allocated cache */
static size_t len    = 0;      /* number of terms stored  */
static size_t n_special = 0;   /* count of 1 or 31 up to current */

/* compute terms up to index n (a(n) for n>=2) */
static void ensure_cache(idx_t n)
{
    if (n < 2)
        return;

    /* initialise first term a(2)=7 if needed */
    if (len == 0) {
        cap = 16;
        cache = seq_alloc(cap);
        cache[0] = (seq_t)7;   /* a(2) */
        len = 1;
        n_special = 0;          /* 7 is neither 1 nor 31 */
    }

    /* append terms until we have a(n) */
    while (len < (size_t)(n - 1)) {
        seq_t cur = cache[len - 1];   /* a(current index) */
        seq_t next;

        /* current index is len+1 (since cache[0]=a(2)) */
        if (cur == (seq_t)1 || cur == (seq_t)31) {
            /* special handling */
            seq_t tmp = (seq_t)(2 + n_special);   /* S(n) = n_special */
            if (tmp == (seq_t)31) {
                next = (seq_t)47;
            } else {
                if (is_prime_big(tmp) && tmp != (seq_t)31)
                    next = odd_part(3 * tmp + 1);
                else if ((tmp & (seq_t)1) && tmp > (seq_t)1)
                    next = odd_part(tmp + 1);
                else
                    next = odd_part(tmp);
            }
        } else {
            /* normal recurrence */
            if (is_prime_big(cur) && cur != (seq_t)31)
                next = odd_part(3 * cur + 1);
            else if ((cur & (seq_t)1) && cur > (seq_t)1)
                next = odd_part(cur + 1);
            else
                next = odd_part(cur);
        }

        /* grow cache if needed */
        if (len == cap) {
            size_t new_cap = cap * 2;
            if (new_cap == 0)
                new_cap = 16;
            cache = seq_realloc(cache, cap, new_cap);
            cap = new_cap;
        }

        /* store the new term */
        cache[len] = next;

        /* update special counter if the new term is 1 or 31 */
        if (next == (seq_t)1 || next == (seq_t)31)
            ++n_special;

        ++len;
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined for n < 2 */
    ensure_cache(n);
    return cache[n - 2];
}
