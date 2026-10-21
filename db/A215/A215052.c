/*
 * A215052  a(n) = (binomial(n,5) - floor(n/5)) / 5.
 * https://oeis.org/A215052
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (binomial(n,5) - floor(n/5)) / 5
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 304, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* For n < 6 the sequence is undefined; return 0. */
    if (n < 6) return 0;

    /* Compute binomial(n,5). The utility function handles the
       arithmetic without overflow for the needed range. */
    seq_t bin5 = binom(n, 5);

    /* floor(n/5) is just integer division. */
    seq_t div5 = n / 5;

    /* Compute (bin5 - div5) / 5. */
    seq_t temp = bin5 - div5;
    return temp / 5;
}
