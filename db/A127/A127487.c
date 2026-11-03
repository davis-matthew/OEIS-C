/*
 * A127487  Primes p such that 6p - 31 and 6p + 31 are also primes.
 * https://oeis.org/A127487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A127487: primes p such that 6p-31 and 6p+31 are also primes.
   The sequence is indexed with offset 1: a(1)=2, a(2)=7, a(3)=13, …   */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* helper functions from the build's utilities */
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

/* static cache of found terms */
static seq_t *seq_cache = NULL;   /* array of seq_t */
static size_t  seq_len   = 0;     /* number of terms stored */
static size_t  seq_cap   = 0;     /* allocated capacity of seq_cache */
static seq_t  last_scan  = (seq_t)0;   /* last candidate examined */

/* expand the cache capacity to hold at least `needed` elements */
static void
ensure_cache_size(size_t needed)
{
    if (needed <= seq_cap)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;

    if (seq_cache == NULL) {
        seq_cache = seq_alloc(new_cap);
    } else {
        seq_cache = seq_realloc(seq_cache, seq_cap, new_cap);
    }
    seq_cap = new_cap;
}

/* return the n-th element of the sequence (n >= 1). */
seq_t
a(idx_t n)
{
    if (n < (idx_t)1)          /* undefined for n < 1; return 0 as a guard */
        return (seq_t)0;

    /* extend the cache until it contains at least n elements */
    while (seq_len < (size_t)n) {
        /* advance to the next candidate integer */
        last_scan = last_scan + (seq_t)1;

        /* skip non‑prime candidates */
        if (!is_prime_big(last_scan))
            continue;

        /* special case: p = 2 */
        if (last_scan == (seq_t)2) {
            ensure_cache_size(seq_len + 1);
            seq_cache[seq_len++] = last_scan;
            continue;
        }

        seq_t a1 = (seq_t)6 * last_scan - (seq_t)31;
        seq_t a2 = (seq_t)6 * last_scan + (seq_t)31;

        if (is_prime_big(a1) && is_prime_big(a2)) {
            ensure_cache_size(seq_len + 1);
            seq_cache[seq_len++] = last_scan;
        }
    }

    return seq_cache[(size_t)n - 1];
}
