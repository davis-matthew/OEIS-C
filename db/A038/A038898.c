/*
 * A038898  Primes p such that 23 is not a square mod p.
 * https://oeis.org/A038898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes p (p ≠ 23) such that 23 is a quadratic non‑residue
 *            modulo p (i.e. Legendre symbol (23|p) = –1)
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 969 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;          /* array of cached primes   */
    static size_t  cache_len = 0;        /* number of stored elements */
    static size_t  cache_cap = 0;        /* allocated capacity       */

    static uint64_t last_checked = 1;    /* start position for next_prime */

    /* Grow the cache only when more terms are requested */
    while (cache_len < n)
    {
        /* Get next prime > last_checked */
        last_checked = next_prime(last_checked);   /* returns 2,3,5,... */

        /* Skip excluded primes */
        if (last_checked == 2 || last_checked == 23)
            continue;

        /* 23 is a quadratic non‑residue modulo last_checked? */
        if (jacobi_symbol(23, last_checked) == -1)
        {
            /* Ensure capacity for one more element */
            if (cache_len + 1 > cache_cap)
                grow_seq_array(&cache, &cache_cap, cache_len + 1);

            /* Store the prime (cast to seq_t) */
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
