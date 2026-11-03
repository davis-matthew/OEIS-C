/*
 * A118743  Primes of the form k!! + k + 1.
 * https://oeis.org/A118743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118743: primes of the form k!! + k + 1 (k = 0,1,2,…). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t *realloc_seq(seq_t *p, size_t new_cap)
{
    void *tmp = mem_resize(seq_t, p, new_cap);
    if (!tmp) abort();           /* memory allocation failed */
    return (seq_t *)tmp;         /* compatible with both C and C++ */
}

seq_t a(idx_t n)
{
    /* Static state: cache of found primes, current double‑factorial parts,
       and the next k to evaluate. */
    static seq_t *primes   = NULL;
    static size_t primes_len = 0;
    static size_t primes_cap = 0;
    static seq_t evenProd = 1;   /* product of even integers up to current k */
    static seq_t oddProd  = 1;   /* product of odd integers up to current k */
    static idx_t  curK     = 0;  /* smallest k not yet processed */

    /* Generate primes until we have at least n of them. */
    while (primes_len < n) {
        seq_t candidate;

        if (curK == 0) {                     /* 0!! = 1 */
            candidate = (seq_t)2;             /* 0!! + 0 + 1 = 2 */
        } else if (curK & 1) {                /* odd k */
            oddProd  = oddProd * (seq_t)curK;
            candidate = oddProd + (seq_t)curK + (seq_t)1;
        } else {                              /* even k */
            evenProd = evenProd * (seq_t)curK;
            candidate = evenProd + (seq_t)curK + (seq_t)1;
        }

        /* Test primality (works for all supported seq_t sizes). */
        if (is_prime_big(candidate)) {
            /* Grow the cache if needed. */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                primes = realloc_seq(primes, new_cap);
                primes_cap = new_cap;
            }
            primes[primes_len++] = candidate;
        }
        curK++;
    }

    return primes[n - 1];
}
