/*
 * A166326  a(n) = prime(n)^(prime(n)+1) - (prime(n)+1)^prime(n).
 * https://oeis.org/A166326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n)^(prime(n)+1) - (prime(n)+1)^prime(n)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 9,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* n-th prime, 1‑based */
    seq_t p_seq = (seq_t)p;
    seq_t p_plus = p_seq + (seq_t)1;    /* prime(n)+1 */

    seq_t term1 = big_pow(p_seq, p_plus);   /* prime(n)^(prime(n)+1) */
    seq_t term2 = big_pow(p_plus, p_seq);   /* (prime(n)+1)^prime(n) */

    return term1 - term2;                    /* difference */
}
