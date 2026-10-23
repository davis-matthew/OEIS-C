/*
 * A128965  a(n) = (n^3 - n)*7^n.
 * https://oeis.org/A128965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n)*7^n (offset 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses helper functions big_pow and seq_mul from the utils library. */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    const seq_t seven = (seq_t)7;         /* base 7 as seq_t */
    const seq_t n_t    = (seq_t)n;        /* index n as seq_t */

    /* compute n^3 - n */
    seq_t n3 = n_t * n_t * n_t;
    seq_t n3_minus_n = n3 - n_t;

    /* compute 7^n (using the provided big_pow helper) */
    seq_t pow7 = big_pow(seven, n_t);

    /* return the product */
    return seq_mul(pow7, n3_minus_n);
}
