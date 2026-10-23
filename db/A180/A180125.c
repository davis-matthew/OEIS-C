/*
 * A180125  Self-convolution of period-doubling sequence A035263.
 * https://oeis.org/A180125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{n-1} b(i) * b(n-i), where b(k) = 1 if the
 *            exponent of 2 dividing k is even, otherwise b(k) = 0
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   twos_exp_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/twos_exp_even.h"

seq_t a(idx_t n) {
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    /* The convolution includes both i and n-i, so each pair
       is counted twice, consistent with the definition. */
    for (idx_t i = 1; i < n; ++i) {
        idx_t j = n - i;
        /* Compute b(i) and b(j) as 0 or 1. */
        bool bi = twos_exp_even(i);
        bool bj = twos_exp_even(j);
        if (bi && bj)
            sum += (seq_t)1;
    }
    return sum;
}
