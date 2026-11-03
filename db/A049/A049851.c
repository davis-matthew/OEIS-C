/*
 * A049851  Concatenate prevprime(n) and n.
 * https://oeis.org/A049851
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate(prev_prime(n), n)
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 974 (2026-11-01)
 * Helpers:   is_prime, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = prev_prime(n) * 10^digits(n) + n */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime.h"
#include "../utils/num_digits0.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
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

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined; return 0 (irrelevant) */
    if (n < 3) return 0;

    /* Compute prev_prime(n) and number of decimal digits of n */
    seq_t p   = (seq_t)prev_prime(n);                 /* previous prime */
    seq_t digs = (seq_t)num_digits0(n);              /* digits of n (uint64_t -> seq_t) */

    /* Compute 10^digits(n) */
    seq_t pow = pow10((idx_t)digs);                  /* pow10 expects idx_t */

    /* Concatenate: prev_prime(n) followed by n */
    return p * pow + n;
}
