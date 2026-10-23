/*
 * A030003  a(n) = (prime(n)-3)*(prime(n)-5)*(prime(n)-7)/48.
 * https://oeis.org/A030003
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-3)*(prime(n)-5)*(prime(n)-7)/48
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 777, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2; for smaller indices return 0. */
    if (n < 2) return 0;

    /* Retrieve the n‑th prime number; prime(1) = 2, prime(2) = 3, … */
    uint32_t p = get_prime(n);

    /* Compute the expression using a wider integer type to avoid overflow
       on small seq_t implementations and then cast the result back to seq_t. */
    unsigned long long term = (unsigned long long)(p - 3) *
                              (unsigned long long)(p - 5) *
                              (unsigned long long)(p - 7);

    term /= 48ULL;
    return (seq_t)term;
}
