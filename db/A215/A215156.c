/*
 * A215156  Primes congruent to {2, 3, 5, 7} mod 17.
 * https://oeis.org/A215156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 994 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence consists of the primes congruent to 2, 3, 5, 7 (mod 17). */
/*  The function generates primes on demand, keeps the ones that satisfy */
/*  the congruence in a dynamic array, and returns the n‑th element.   */

#include <stdbool.h>
#include <stdlib.h>
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

/* Static cache of primes that meet the congruence requirement.  */
static seq_t *buf = NULL;   /* array of primes found */
static size_t cap = 0;      /* current buffer capacity   */
static size_t count = 0;    /* number of stored primes   */
static seq_t last = (seq_t)1; /* last tested candidate */

/* Return the n-th prime that ≡ 2, 3, 5, or 7 (mod 17);   n ≥ 1.   */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety for n=0 */

    /* Lazy initialization of cache. */
    if (buf == NULL) {
        cap = 16;
        buf = seq_alloc(cap);
        if (!buf) abort();
    }

    /* Generate primes until we have at least n of them. */
    while (count < (size_t)n) {
        /* Advance to next integer candidate. */
        ++last;

        /* Quick primality test. */
        if (!is_prime_big(last))
            continue;

        /* Check the required residue modulo 17. */
        seq_t r = last % (seq_t)17;
        if (r == (seq_t)2 || r == (seq_t)3 ||
            r == (seq_t)5 || r == (seq_t)7) {

            /* Resize the buffer if full. */
            if (count >= cap) {
                size_t new_cap = cap << 1;
                seq_t *tmp = seq_realloc(buf, cap, new_cap);
                if (!tmp) abort();
                buf = tmp;
                cap = new_cap;
            }

            buf[count++] = last;
        }
    }

    return buf[n - 1];
}
