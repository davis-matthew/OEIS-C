/*
 * A174821  Numbers n such that n^4 - n^2 - 1 is prime.
 * https://oeis.org/A174821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Function for OEIS A174821: numbers n such that n^4 - n^2 - 1 is prime
 * Formula: a(n) is the n‑th integer >1 for which n^4 - n^2 - 1 is prime.
 *
 * The implementation keeps a static cache, extending it as needed, so
 * that repeated calls are fast and independent of order.  It uses
 * libutils helpers so it works for all supported seq_t types
 * (builtin integers down to 8 bits, or arbitrary‑precision classes).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Return the n-th element of sequence A174821 (offset 1). */
seq_t a(idx_t n)
{
    /* Static cache data */
    static seq_t *cache = NULL;      /* array of found terms    */
    static idx_t cache_cap = 0;      /* capacity of cache      */
    static idx_t found = 0;          /* number of terms stored */
    static idx_t last_checked = 1;   /* last candidate checked */

    /* Grow cache if necessary */
    if (cache_cap < n) {
        size_t new_cap = (cache_cap == 0) ? n : (cache_cap * 2);
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) return (seq_t)0;   /* memory error – unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Search for additional terms until we have at least n of them */
    while (found < n) {
        ++last_checked;                   /* next candidate integer */
        seq_t m   = (seq_t)last_checked;  /* cast to seq_t */

        /* Compute m^4 - m^2 - 1 */
        seq_t sq   = m * m;           /* m^2          */
        seq_t term = sq * sq - sq - (seq_t)1; /* m^4 - m^2 - 1 */

        /* Test for primality of term */
        if (is_prime_big(term)) {
            /* Store the candidate as the next sequence element */
            cache[found++] = m;
        }
    }

    /* Return the requested element (offset 1) */
    return cache[n - 1];
}
