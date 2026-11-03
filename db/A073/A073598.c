/*
 * A073598  Numbers n such that n^3 + 5 is prime.
 * https://oeis.org/A073598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 362, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 986 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for the nth integer n such that n³ + 5 is prime. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* Static cache of found values */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  len   = 0;

    if (n == 0)
        return 0; /* undefined offset; guard against accidental misuse */

    /* Starting point for the search: one after the last stored value
       (or 1 if none stored yet). */
    idx_t cur_n = (len == 0) ? 1 : (idx_t)(cache[len - 1] + 1);

    while (len < n) {
        seq_t sn   = (seq_t)cur_n;                 /* current candidate */
        seq_t cube = sn * sn * sn + (seq_t)5;       /* n³ + 5 */

        if (isPrimeGeneric(cube)) {                /* test primality */
            /* Grow cache if needed */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 4 : cap << 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = sn;                      /* store qualifying n */
        }
        cur_n++;  /* next candidate integer */
    }

    return cache[n - 1]; /* zero‑based indexing */
}
