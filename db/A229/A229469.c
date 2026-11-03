/*
 * A229469  Numbers k such that T(k) + S(k) + 1 is prime, where T(k) and S(k)
 *          are the k-th triangular and square numbers.
 * https://oeis.org/A229469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k such that k*(k+1)/2 + k^2 + 1 is prime
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 4922, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9378 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  next_k   = 0;   /* previously checked k (0 means none) */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, but be safe for n==0 */
        return 0;

    /* generate terms until we have at least n entries */
    while (cache_len < (size_t)n)
    {
        ++next_k;
        seq_t k = (seq_t)next_k;

        /* expr = T(k) + S(k) + 1 = k*(k+1)/2 + k^2 + 1 */
        seq_t expr = (k * (k + 1)) / 2 + (k * k) + 1;

        if (isPrimeGeneric(expr))
            append_candidate(&cache, &cache_len, &cache_cap, k);
    }

    return cache[n - 1];
}
