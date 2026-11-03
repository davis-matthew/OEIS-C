/*
 * A187677  Primes of the form 8*k^2 + 6*k - 1 for positive k.
 * https://oeis.org/A187677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*n^2 + 6*n - 1 for prime n, but implemented by testing
 *            the polynomial for successive k until the n‑th prime of that
 *            form is found
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* Static cache storing primes found so far. */
    static seq_t *primes   = NULL;   /* array of seq_t */
    static size_t primes_cnt = 0;    /* number of stored primes */
    static size_t primes_cap = 0;    /* current capacity of array */
    static idx_t next_k = 1;         /* next k to test */

    /* Undefined behavior for n == 0 – callers may assume >= 1. */
    if (n == 0)
        return 0;

    /* Allocate the cache on first use. */
    if (primes == NULL) {
        primes_cap = 16;
        primes = seq_alloc(primes_cap);
        if (primes == NULL)          /* out of memory – return failure. */
            return 0;
    }

    /* Generate candidates until we have at least n primes. */
    while (primes_cnt < n) {
        /* Compute candidate value: 8*k^2 + 6*k - 1 using idx_t to avoid
           overflow for typical inputs.  The result is small enough to
           cast to seq_t for storage and primality test. */
        idx_t k = next_k;
        idx_t val_i = 8 * k * k + 6 * k - 1;
        seq_t val = (seq_t)val_i;

        if (is_prime_big(val)) {
            /* Grow the array if necessary. */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap * 2;
                seq_t *tmp = seq_realloc(primes, primes_cap, new_cap);
                if (tmp == NULL)
                    return 0;      /* allocation failure */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = val;
        }

        ++next_k;
    }

    return primes[n - 1];
}
