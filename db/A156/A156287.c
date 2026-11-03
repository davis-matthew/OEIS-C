/*
 * A156287  Numbers k such that 4*k-5 is a prime number.
 * https://oeis.org/A156287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = kth integer k such that 4*k-5 is prime
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 985 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;
static size_t seq_cache_cap = 0;
static seq_t seq_last_k = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety for 0-index calls */

    if (seq_cache_len >= (size_t)n) {
        return seq_cache[n - 1];
    }

    if (!seq_cache) {
        seq_cache_cap = 16;
        seq_cache = alloc_seq(seq_cache_cap);
    }

    while (seq_cache_len < (size_t)n) {
        seq_last_k++;  /* next integer to test */

        if (seq_last_k >= 2) {          /* 4*k-5 must be ≥3 for a prime candidate */
            seq_t check = mul_small(seq_last_k, 4);
            check -= 5;
            if (isPrimeGeneric(check)) {
                if (seq_cache_len == seq_cache_cap) {
                    grow_seq(&seq_cache, &seq_cache_cap, seq_cache_len + 1);
                }
                seq_cache[seq_cache_len++] = seq_last_k;
            }
        }
    }

    return seq_cache[n - 1];
}
