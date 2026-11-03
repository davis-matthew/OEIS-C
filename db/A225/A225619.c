/*
 * A225619  Composite numbers that remain composite if any digit is deleted
 *          (zero and one are not considered prime).
 * https://oeis.org/A225619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th composite number such that deleting any single digit
 *            leaves a number that is composite (0 and 1 are not considered
 *            prime)
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9525 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, num_digits
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
#include "../utils/num_digits.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

static seq_t *cache = NULL;        /* gathered terms */
static size_t cache_len = 0;       /* number of stored terms */
static size_t cache_cap = 0;       /* current allocation size */
static idx_t next_val = 0;         /* next integer to test (as idx_t) */

/* Helper: test if a number is composite (i.e., >1 and not prime). */
static bool is_composite(seq_t v)
{
    if (v <= 1)
        return false;
    return !isPrimeGeneric(v);
}

seq_t a(idx_t n)
{
    /* Build the cache until we have at least n terms. */
    while (cache_len < n) {
        seq_t x = (seq_t)next_val++;   /* candidate number */

        if (!is_composite(x))
            continue;                  /* skip primes and ≤1 */

        bool ok = true;
        int len = num_digits(x);

        /* Check deletions of all digit positions. */
        for (int pos = 0; pos < len && ok; ++pos) {
            seq_t pow10pos  = pow10((idx_t)pos);
            seq_t pow10next = pow10((idx_t)(pos + 1));

            seq_t low  = x % pow10pos;
            seq_t high = x / pow10next;
            seq_t deleted = high * pow10pos + low;

            if (!is_composite(deleted))
                ok = false;
        }

        if (ok) {
            /* Resize cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }
    }
    return cache[n - 1];
}
