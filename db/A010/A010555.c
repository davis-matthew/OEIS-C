/*
 * A010555  a(n) = 1 if n is the product of an even number of distinct primes,
 *          otherwise a(n) = -1.
 * https://oeis.org/A010555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 100000, i16 n <= 100000, i32 n <= 100000,
 *            i64 n <= 100000, i128 n <= 100000, big any n
 * Validated: standard, pass, 95 terms through n = 9874 (2026-11-01)
 * Helpers:   bigomega, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010555: a(n) = 1 if n is a product of an even number of distinct
   primes, otherwise a(n) = -1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* defined only for n ≥ 1 */

    int distinct = omega(n);   /* number of distinct prime divisors */
    seq_t omega_seq = bigomega(n);             /* total number of prime factors (with multiplicity) */
    int total = (int)omega_seq;                /* ω(n) fits in a small integer for 64‑bit n */

    if (total > distinct)                     /* not square‑free → odd number of distinct primes */
        return (seq_t)-1;

    return (distinct % 2 == 0) ? (seq_t)1 : (seq_t)-1;
}
