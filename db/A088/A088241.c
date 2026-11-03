/*
 * A088241  Values of y, where x^2 + xy + y^2 = p (x<y) is a prime of the form
 *          6n + 1 (A002476).
 * https://oeis.org/A088241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 75, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9398 (2026-11-03)
 * Helpers:   ipow_i32, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088241: a(n) is the y‑coordinate in the unique
   representation x² + xy + y² = p for the nth prime p ≡ 1 (mod 6)
   with x < y.   The function builds the needed primes incrementally. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    static seq_t *prime_cache = NULL;
    static size_t prime_cache_len = 0;
    static size_t prime_cache_cap = 0;
    static idx_t last_checked = 7;   /* start scanning from the first
                                       prime of the form 6k+1 */

    /* Generate primes of the form 6k+1 until we have n of them. */
    while (prime_cache_len < n) {
        idx_t m = last_checked;
        if (is_prime(m) && (m % 6u == 1u)) {
            append_candidate(&prime_cache, &prime_cache_len, &prime_cache_cap,
                             (seq_t)m);
        }
        last_checked = m + 1;
    }

    /* Fetch the nth prime (0‑based index). */
    seq_t p = prime_cache[n - 1];

    /* Search for y in x² + xy + y² = p.  y < sqrt(p). */
    seq_t y = floor_sqrt(p);
    for (; y > 0; --y) {
        seq_t ysq = y * y;
        seq_t left  = 3 * ysq;          /* 3*y² */
        seq_t right = 4 * p;            /* 4*p  */
        if (left > right)              /* discriminant would be negative */
            continue;
        seq_t D = right - left;
        seq_t s = floor_sqrt(D);
        if (s * s == D && s > y && ((s - y) % 2u) == 0u) {
            return y;                  /* unique y found */
        }
    }

    /* In theory we should never reach here. */
    return 0;
}
