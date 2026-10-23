/*
 * A157057  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 16.
 * https://oeis.org/A157057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..min(n,8)} C(n+1,i) * C(7,i-1) * C(n-i+8,8)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 59, u128 n <= 963,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 1: a(1) = 2) */

#include <stdint.h>    /* for uint64_t */

static seq_t binom_compute(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i);
        r = r / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    seq_t result = (seq_t)0;
    idx_t upper = (n < 8 ? n : 8);
    for (idx_t i = 1; i <= upper; ++i) {
        seq_t term = binom_compute(n + 1, i);
        term *= binom_compute(7, i - 1);
        term *= binom_compute(n - i + 8, 8);
        result += term;
    }
    return result;
}
