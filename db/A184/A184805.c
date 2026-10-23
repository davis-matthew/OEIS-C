/*
 * A184805  Primes of the form floor(k*s), where s=(5+sqrt(5))/4.
 * https://oeis.org/A184805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is defined as the n-th prime of the Beatty sequence
   floor(k * s) where s = (5 + sqrt(5)) / 4.  The function below
   computes a(n) for any n >= 1 by iterating over k, evaluating
   floor(k*s) exactly with integer arithmetic, testing the result
   for primality, and returning the n-th prime encountered. */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"          /* floor_sqrt(seq_t x) */
#include "../utils/is_prime_big.h"        /* is_prime_big(seq_t n) */

/* Compute floor(k * s) with s = (5 + sqrt(5)) / 4, using only integer
   arithmetic.  For an integer k,
        k * s = (5k + k * sqrt(5)) / 4
   and floor(k * sqrt(5)) = floor(sqrt(5 * k^2)). */
static seq_t floor_k_s(idx_t k)
{
    seq_t k_seq = (seq_t)k;                    /* convert k to seq_t */
    seq_t k_sq   = k_seq * k_seq;              /* k^2                      */
    seq_t tmp    = k_sq * (seq_t)5;            /* 5 * k^2                  */
    seq_t sqrt_k = floor_sqrt(tmp);            /* floor(k * sqrt(5))       */
    seq_t num    = k_seq * (seq_t)5 + sqrt_k;  /* 5k + floor(k*sqrt(5))    */
    return num / (seq_t)4;                     /* floor division by 4      */
}

/* Return the n-th term of the sequence (1‑based).  The function
   iterates over k, evaluates floor(k*s), checks primality,
   and returns the n‑th prime found. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for n >= 1; return zero for safety. */
        return (seq_t)0;
    }

    idx_t count = 0;
    idx_t k = 1;

    while (true) {
        seq_t val = floor_k_s(k);
        if (is_prime_big(val)) {
            ++count;
            if (count == n) {
                return val;
            }
        }
        ++k;
    }
}
