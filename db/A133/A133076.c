/*
 * A133076  Successive digits of isolated primes A007510(n).
 * https://oeis.org/A133076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, less
 * Formula:   successive digits of isolated primes A007510(n)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Static cache of digits of isolated primes */
static seq_t *digits = NULL;
static size_t digits_len = 0;
static size_t digits_cap = 0;
static uint64_t last_isolated = 0;

/* Append the decimal digits of p to the cache */
static void append_digits(uint64_t p)
{
    if (p == 0) return;                /* p is always > 0 for primes */

    /* Temporary buffer for digits (max 20 digits fit in uint64_t) */
    uint64_t tmp[20];
    size_t cnt = 0;
    while (p > 0) {
        tmp[cnt++] = p % 10;
        p /= 10;
    }

    /* Append in correct order (most‑significant first) */
    for (size_t i = cnt; i > 0; --i) {
        if (digits_len == digits_cap) {
            size_t new_cap = digits_cap == 0 ? 16 : digits_cap * 2;
            seq_t *new_ptr = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (new_ptr == NULL) {
                /* Memory allocation failed; abort */
                exit(1);
            }
            digits = new_ptr;
            digits_cap = new_cap;
        }
        digits[digits_len++] = (seq_t)tmp[i - 1];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) /* Offset is 1; n=0 not used but handled for completeness */
        return 0;

    /* Extend cache until it contains at least n digits */
    if (digits_len < (size_t)n) {
        while (digits_len < (size_t)n) {
            for (uint64_t cand = last_isolated + 1;; ++cand) {
                if (is_prime(cand) && !is_prime(cand - 2) && !is_prime(cand + 2)) {
                    last_isolated = cand;
                    append_digits(cand);
                    break; /* next isolated prime found */
                }
            }
        }
    }

    return digits[(size_t)n - 1];
}
