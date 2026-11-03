/*
 * A217674  Primes p = a^2 + b^2 such that (x - a)^2 + (y - b)^2 = 2 and q - p
 *          = 4, where q = x^2 + y^2 is prime, assuming that a > b > 0 and x >
 *          y > 0.
 * https://oeis.org/A217674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes p = 2b^2 + 2b + 1, b ≥ 2, such that both p and p +
 *            4 are prime (OEIS A217674)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 310, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 48 terms through n = 914 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Sequence entry a(n) with offset 1. */
seq_t a(idx_t n) {
    /* Static cache of computed terms. */
    static seq_t *seq = NULL;
    static size_t capacity = 0;
    static size_t length = 0;
    static idx_t current_b = 2;   /* smallest b with p = 13. */

    if (n == 0) return 0;  /* undefined, but harmless */

    /* Generate terms until the n‑th term is available. */
    while (length < n) {
        /* Compute p = 2*b^2 + 2*b + 1. */
        seq_t bb = (seq_t)current_b * current_b;
        seq_t p  = 2 * bb + 2 * (seq_t)current_b + 1;

        if (isPrimeGeneric(p)) {
            seq_t q = p + 4;
            if (isPrimeGeneric(q)) {
                /* Store p in the cache. */
                if (length == capacity) {
                    size_t new_cap = capacity ? capacity * 2 : 16;
                    seq_t *tmp = mem_resize(seq_t, seq, new_cap);
                    if (!tmp) abort();          /* allocation failure */
                    seq = tmp;
                    capacity = new_cap;
                }
                seq[length++] = p;
            }
        }
        ++current_b;
    }

    return seq[n - 1];
}
