/*
 * A155770  Primes of form: 2*n^2+2*n-41.
 * https://oeis.org/A155770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 792 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes of the form 2*n^2 + 2*n - 41
   a(n) is the n‑th prime of this form (a(1) = 19), offset = 1.  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
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

/* Helper routines that work for both C and C++ builds.  */
#ifdef __cplusplus
static seq_t* allocate_seq(size_t n)
{
    return new seq_t[n];
}
static seq_t* reallocate_seq(seq_t* old, size_t old_cap, size_t new_cap)
{
    seq_t* newptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        newptr[i] = old[i];
    delete[] old;
    return newptr;
}
#else
static seq_t* allocate_seq(size_t n)
{
    return (seq_t*)malloc(n * sizeof(seq_t));
}
static seq_t* reallocate_seq(seq_t* old, size_t old_cap, size_t new_cap)
{
    (void)old_cap;          /* unused in C */
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
}
#endif

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t   found  = 0;      /* number of terms already stored   */
    static idx_t   next_n = 5;      /* n whose value 2*n^2+2*n-41 first
                                       might be prime (a(1)=19).         */

    /* initialise cache on the first call */
    if (cache == NULL) {
        cap = 8;
        cache = allocate_seq(cap);
        if (cache == NULL) return 0;   /* allocation failure */
    }

    /* fill cache up to at least the requested term */
    while (found < n) {
        /* compute candidate value as seq_t to avoid overflow */
        seq_t nn = (seq_t)next_n;
        seq_t val = 2 * nn * (nn + 1) - 41;

        if ((val > 1) && is_prime_big(val)) {
            if (found == cap) {
                size_t new_cap = cap << 1;
                seq_t* tmp = reallocate_seq(cache, cap, new_cap);
                if (tmp == NULL) return 0;   /* reallocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[found++] = val;
        }
        ++next_n;
    }

    return cache[n - 1];
}
