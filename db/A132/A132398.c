/*
 * A132398  Numbers n such that 11*n^2 + 1 is prime.
 * https://oeis.org/A132398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Computes the n-th integer m such that 11*m^2 + 1 is prime. All
 *            terms are multiples of 6. This implementation lazily generates
 *            terms on demand and caches them for future calls
 * Range:     u8 none, u16 n <= 5, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Return the n-th element (1‑based) of sequence A132398. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for 0, but avoid UB */

    /* Static state shared across calls */
    static seq_t *cache = NULL;          /* array of found terms   */
    static size_t capacity = 0;          /* allocated length       */
    static size_t count = 0;             /* number of terms cached */
    static idx_t last_k = 0;             /* last processed multiplier (k for m=6*k) */

    /* Extend cache until we have at least n terms */
    while (count < n) {
        ++last_k;                                     /* next multiplier   */
        seq_t m = (seq_t)(6 * last_k);                /* candidate number  */
        seq_t val = (seq_t)11 * m * m + 1;            /* 11*m^2+1 */

        if (isPrimeGeneric(val)) {                    /* primality test    */
            /* Grow the cache if needed */
            if (count == capacity) {
                size_t new_cap = capacity ? capacity * 2 : 8;
                grow_seq(&cache, &capacity, new_cap);
            }
            cache[count++] = m;                        /* store term        */
        }
    }

    return cache[n - 1];
}
