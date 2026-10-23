/*
 * A136546  a(n) = Sum_{k=1..floor(n*log(2))} (1 + (n - floor(n/2^k)) * (n - 1
 *          - floor(n/2^(k - 1))) mod 2).
 * https://oeis.org/A136546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = Sum_{k=1..floor(n*log(2))} ((1 + (n - floor(n/2^k)) * (n
 *            - 1 - floor(n/2^(k - 1)))) mod 2)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the upper bound kmax = floor(n*log(2)). */
    const double LN2 = 0.6931471805599453094172321214581765680755001343602552;
    idx_t kmax = (idx_t)((double)n * LN2 + 1e-12);

    /* Count how many k make the product of the two factors odd. */
    int64_t odd_both = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t floor_k;
        idx_t floor_km1;

        if (k < 64)
            floor_k = n >> k;
        else
            floor_k = 0;

        if (k - 1 < 64)
            floor_km1 = n >> (k - 1);
        else
            floor_km1 = 0;

        int a_parity = ((int64_t)n - (int64_t)floor_k) & 1;
        int b_parity = ((int64_t)n - 1 - (int64_t)floor_km1) & 1;
        if (a_parity && b_parity)
            ++odd_both;
    }

    /* Each term equals 1 iff the product of the two factors is even. */
    seq_t result = (seq_t)kmax - (seq_t)odd_both;
    return result;
}
