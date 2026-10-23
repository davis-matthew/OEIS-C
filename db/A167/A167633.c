/*
 * A167633  Number of ordered n-tuples of positive integers such that the
 *          largest value is n and the first value is odd.
 * https://oeis.org/A167633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n/2) * n^(n-1) - ceil((n-1)/2) * (n-1)^(n-1) where
 *            ceil(x/2) is computed as (x+1)/2 for unsigned integers. This
 *            counts ordered n‑tuples of positive integers with largest
 *            element n and first element odd
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Count of odd numbers in [1..n] */
    const seq_t odd_all = (seq_t)(n + 1) / 2;

    /* Count of odd numbers in [1..n-1] = ceil((n-1)/2) = n/2 */
    const seq_t odd_less = (seq_t)(n) / 2;

    const seq_t pow_n   = pow_seq((seq_t)n, n - 1);
    const seq_t pow_n1  = pow_seq((seq_t)n - 1, n - 1);

    return odd_all * pow_n - odd_less * pow_n1;
}
