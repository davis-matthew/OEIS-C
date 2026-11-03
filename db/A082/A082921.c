/*
 * A082921  Sort the digits of these squares into descending order and drop
 *          zeros to get primes.
 * https://oeis.org/A082921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th perfect square whose digits, when reordered in
 *            descending order and with all zeros removed, form a prime number
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 54 terms through n = 947 (2026-11-03)
 * Helpers:   digit_frequencies, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/digit_frequencies.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t  next_k = 1;           /* next integer to test */

    while (len < n) {
        seq_t m = (seq_t)next_k * (seq_t)next_k;  /* square */

        unsigned int freq[10];
        digit_frequencies(m, freq);              /* count digits 0–9 */

        seq_t r = 0;
        for (int d = 9; d >= 1; --d) {           /* build digits descending, skip 0 */
            for (unsigned int c = 0; c < freq[d]; ++c)
                r = r * 10 + d;
        }

        if (isPrimeGeneric(r)) {                 /* keep only qualifying squares */
            if (len + 1 > cap) {                 /* grow the cache */
                size_t new_cap = cap ? cap * 2 : 1;
                cache = seq_realloc(cache, cap, new_cap);
                cap = new_cap;
            }
            cache[len++] = m;
        }

        ++next_k;
    }

    return cache[n-1];
}
