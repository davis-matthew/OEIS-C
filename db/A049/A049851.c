/*
 * A049851  Concatenate prevprime(n) and n.
 * https://oeis.org/A049851
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate(prev_prime(n), n)
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_count, pow10, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = prev_prime(n) * 10^digits(n) + n */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/prev_prime.h"
#include "../utils/digit_count.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined; return 0 (irrelevant) */
    if (n < 3) return 0;

    /* Compute prev_prime(n) and number of decimal digits of n */
    seq_t p   = (seq_t)prev_prime(n);                 /* previous prime */
    seq_t digs = (seq_t)digit_count(n);              /* digits of n (uint64_t -> seq_t) */

    /* Compute 10^digits(n) */
    seq_t pow = pow10((idx_t)digs);                  /* pow10 expects idx_t */

    /* Concatenate: prev_prime(n) followed by n */
    return p * pow + n;
}
