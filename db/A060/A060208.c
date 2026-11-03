/*
 * A060208  a(n) = 2*pi(n) - pi(2*n), where pi(i) = A000720(i).
 * https://oeis.org/A060208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*prime_pi(n) - prime_pi(2*n)
 * Range:     i8 n <= 5716, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 138 terms through n = 9849 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = 2*pi(n) - pi(2*n) where pi(x) is the prime‑counting function. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n == 0 return 0 to keep the function safe. */
    if (n == 0) return 0;
    seq_t pi_n  = prime_pi(n);
    seq_t pi_2n = prime_pi(n * (idx_t)2);
    return pi_n * (seq_t)2 - pi_2n;
}
