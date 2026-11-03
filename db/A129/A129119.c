/*
 * A129119  Numbers of the form 2*p (with p a prime number) such that p^2+4 is
 *          prime.
 * https://oeis.org/A129119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129119:
 * Numbers of the form 2*p where p is prime and p^2 + 4 is also prime.
 * Computation: iterate over odd candidates, test primality, and
 * cache the 2*p values.  This is a straightforward prime‑search
 * implementation suitable for n up to ~10 000.  The algorithm uses
 * the provided utilities:
 *   - is_prime(idx_t)      – test 64‑bit primes
 *   - is_prime_big(seq_t) – test arbitrary‑precision primes
 *   - seq_alloc(size_t)   – allocate seq_t[][] array
 *   - seq_realloc(seq_t*,size_t,size_t) – resize the array
 *   - grow_seq(&arr,&cap,need) – utility that resizes to at least *need*
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return the nth term of A129119, 1‑based indexing. */
seq_t a(idx_t n)
{
    /* cache holds the terms found so far */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of stored terms */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* next odd integer to test for primality (starting at 3) */
    static uint64_t next_candidate = 3;

    /* Allocate initial cache if needed */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = seq_alloc(cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        uint64_t p = next_candidate;

        if (is_prime((idx_t)p)) {
            /* compute p^2 + 4 in seq_t arithmetic */
            seq_t sq = (seq_t)p * (seq_t)p + (seq_t)4;
            if (is_prime_big(sq)) {
                /* store 2*p */
                if (cache_size >= cache_cap) {
                    grow_seq(&cache, &cache_cap, cache_size + 1);
                }
                cache[cache_size++] = (seq_t)2 * (seq_t)p;
            }
        }

        /* advance to the next odd candidate */
        next_candidate = p + 2;
    }

    return cache[(size_t)n - 1];
}
