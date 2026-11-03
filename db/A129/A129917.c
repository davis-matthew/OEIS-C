/*
 * A129917  Numbers k such that adding the sum of the digits of k to the
 *          product of k and the product of its digits yields a prime.
 * https://oeis.org/A129917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) is the n-th positive integer k such that k*digit_product(k)
 *            + digit_sum(k) is prime
 * Range:     u8 n <= 8, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   digit_product, digit_sum, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"
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
    static seq_t *seq = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;
    static idx_t  current_k = 1;   /* next integer to test */

    if (n == 0) return 0;          /* safety: n>=1 in OEIS, below offset ignored */

    /* Grow the sequence until it contains the n-th value. */
    while (len < n) {
        seq_t k   = (seq_t)current_k;
        seq_t prod = digit_product(k);
        seq_t sum  = digit_sum(k);
        seq_t val  = k * prod + sum;

        if (isPrimeGeneric(val)) {
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 2 : cap * 2;
                seq_t *tmp = mem_resize(seq_t, seq, new_cap);
                if (!tmp) {
                    /* Out of memory – abort sequence generation. */
                    abort();
                }
                seq = tmp;
                cap = new_cap;
            }
            seq[len++] = k;        /* store the found term */
        }
        ++current_k;
    }

    return seq[n - 1];
}
