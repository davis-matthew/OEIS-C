/*
 * A081515  Sum of terms in n-th row of A081517.
 * https://oeis.org/A081515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = next prime integer >= n*(n+1)/2, with a(1) = 1
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081515 - Sum of terms in n‑th row of A081517 */

#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* special‑case n == 1 */
    if (n <= 1)
        return (seq_t)1;

    /* compute the n‑th triangular number in seq_t arithmetic */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* find the smallest prime >= t */
    seq_t p = t;
    while (!is_prime_big(p))
        ++p;

    return p;
}
