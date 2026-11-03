/*
 * A166563  Numbers n such that 12*n+5 is not prime.
 * https://oeis.org/A166563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer m such that 12*m + 5 is composite
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 997 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* static table of found terms (zero‑based indexing) */
    static seq_t *cache    = NULL;   /* array of m values */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t last_checked = 0;   /* largest m examined so far */

    if (n == 0) /* offset is 1, but guard against zero for safety */
        return 0;

    /* Grow cache until it contains at least n terms */
    while (cache_len < n) {
        last_checked += 1; /* next candidate m to test */

        /* Compute 12*m + 5 as a seq_t value before primality test */
        seq_t test = (seq_t)12 * last_checked + 5;

        if (!isPrimeGeneric(test)) { /* composite -> record this m */
            /* Ensure enough space for one more element */
            grow_seq(&cache, &cache_cap, cache_len + 1);

            /* Store the term as a seq_t (conversion from idx_t is safe) */
            cache[cache_len] = (seq_t)last_checked;
            cache_len += 1;
        }
    }

    return cache[n - 1]; /* return the nth term */
}
