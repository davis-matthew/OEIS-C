/*
 * A090266  Least k such that concatenation k, prime(n), k is a prime.
 * https://oeis.org/A090266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 4, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   concat_parts, digits, get_prime, is_prime_big, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A090266:  a(n) = least k ≥ 1 such that concat(k, prime(n), k) is prime. 
   The value is found by searching successive k and testing primality of the 
   concatenated number. */
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; below that return 0. */
    if (n == 0) return 0;

    /* nth prime, with prime(1) = 2. */
    seq_t p = (seq_t)get_prime(n);

    /* Search for the smallest k such that concat(k, p, k) is prime. */
    for (idx_t k = 1;; ++k)
    {
        /* (k || p)  as seq_t */
        seq_t temp = concat_parts(k, (idx_t)p);

        /* (k || p) || k  =  temp * 10^digits(k) + k  */
        seq_t kseq   = (seq_t)k;
        seq_t pow10k = pow10((idx_t)digits(k));
        seq_t cand   = seq_mul(temp, pow10k) + kseq;

        if (is_prime_big(cand))
            return kseq;           /* k itself is the requested value */
    }
}
