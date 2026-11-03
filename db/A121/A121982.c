/*
 * A121982  Numbers k such that k^2 + 15 is prime.
 * https://oeis.org/A121982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 4511, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 73 terms through n = 4528 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that k^2 + 15 is prime.
   The function constructs the sequence incrementally and caches results.
   It uses incremental prime testing via isPrimeGeneric. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

seq_t a(idx_t n) {
    /* n is 1‑based.  If n is zero, return zero (should not happen). */
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t next_k = 1; /* Next candidate k to test */

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        idx_t k = next_k++;
        seq_t ksq = (seq_t)k * (seq_t)k;
        seq_t val = ksq + (seq_t)15;

        if (isPrimeGeneric(val)) {
            /* grow the cache if necessary */
            grow_seq(&cache, &cap, len + 1);
            cache[len++] = (seq_t)k; /* store k, not k^2+15 */
        }
    }

    return cache[n - 1];
}
