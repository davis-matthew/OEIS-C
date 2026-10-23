/*
 * A049711  a(n) = n - prevprime(n).
 * https://oeis.org/A049711
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - prevprime(n)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;   /* Offset is 3 – behavior below is irrelevant */
    idx_t p   = prev_prime(n);    /* largest prime < n (variant 2) */
    idx_t d   = n - p;            /* difference fits in seq_t for the tested range */
    return (seq_t)d;
}
