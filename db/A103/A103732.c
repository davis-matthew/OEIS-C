/*
 * A103732  Negative of column k=5 sequence of array A103728.
 * https://oeis.org/A103732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A103732
   Formula: a(n) = ((p(n+4)-1)*C(p(n+4)-1,5) - 1)/p(n+4) ,
   where p(k) is the k‑th prime (A000040).  This equals the negative
   of the column‑k=5 entry of array A103728. */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Compute the binomial coefficient C(t,5) for a seq_t t using
   the multiplicative formula to keep intermediate values small. */
static seq_t
binom5(seq_t t)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= 5; ++i) {
        res *= t - (seq_t)(i - 1);
        res /= (seq_t)i;
    }
    return res;
}

/* Return the n-th term of sequence A103732. */
seq_t
a(idx_t n)
{
    /* Index of the prime we need is n+4 */
    idx_t prime_index = n + 4;

    /* p = p(n+4) */
    seq_t p = get_prime(prime_index);

    /* t = p - 1 */
    seq_t t = p - 1;

    /* bin = C(t,5) */
    seq_t bin = binom5(t);

    /* numerator = t * bin - 1 */
    seq_t num = t * bin;
    num -= 1;

    /* result = num / p  (exact division) */
    seq_t res = num / p;

    return res;
}
