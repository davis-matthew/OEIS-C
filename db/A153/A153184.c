/*
 * A153184  Numbers n such that 3*n-2 is not prime.
 * https://oeis.org/A153184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th positive integer k such that 3*k-2 is not prime
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

/* Computes the n-th term of sequence A153184.
   a(n) is the n-th positive integer k with (3*k-2) not prime. */
seq_t a(idx_t n)
{
    idx_t k      = 1;          /* candidate integer */
    idx_t count  = 0;          /* how many valid terms found so far */

    while (1) {
        seq_t val = (seq_t)k * (seq_t)3 - (seq_t)2; /* compute 3*k-2 in seq_t */
        if (!is_prime_big(val)) {                  /* 3*k-2 is composite (or 1) */
            ++count;
            if (count == n)
                return (seq_t)k;   /* found the n-th term */
        }
        ++k;
    }
}
