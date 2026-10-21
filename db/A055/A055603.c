/*
 * A055603  Number of n X n binary matrices with no zero rows or columns and
 *          with n+2 ones.
 * https://oeis.org/A055603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*(9*n^4+42*n^3+7*n^2-122*n-120)*n!/576, n >= 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the above formula using a cancellation
   strategy that keeps intermediate values within the capacity of
   the chosen seq_t type for all n for which a(n) fits in seq_t. */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case n = 1 */
    if (n == 1) return 0;

    /* Remaining factors to cancel from 576 = 2^6 * 3^2 */
    unsigned twos  = 6;
    unsigned threes = 2;

    seq_t result = 1;

    /* Factorial part: multiply 2 .. n, cancelling 2s and 3s */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t prod = (seq_t)i;

        while (twos > 0 && (prod & 1) == 0) {
            prod >>= 1;
            twos--;
        }
        while (threes > 0 && prod % 3 == 0) {
            prod /= 3;
            threes--;
        }

        result *= prod;
    }

    /* Multiply by (n-1) */
    seq_t prod_nm1 = (seq_t)(n - 1);
    while (twos > 0 && (prod_nm1 & 1) == 0) {
        prod_nm1 >>= 1;
        twos--;
    }
    while (threes > 0 && prod_nm1 % 3 == 0) {
        prod_nm1 /= 3;
        threes--;
    }
    result *= prod_nm1;

    /* Multiply by n itself (n appears twice in the formula) */
    seq_t prod_n = (seq_t)n;
    while (twos > 0 && (prod_n & 1) == 0) {
        prod_n >>= 1;
        twos--;
    }
    while (threes > 0 && prod_n % 3 == 0) {
        prod_n /= 3;
        threes--;
    }
    result *= prod_n;

    /* Compute polynomial part: 9*n^4 + 42*n^3 + 7*n^2 - 122*n - 120 */
    seq_t s_n = (seq_t)n;
    seq_t n2 = s_n * s_n;
    seq_t n3 = n2 * s_n;
    seq_t n4 = n3 * s_n;

    seq_t poly = 9 * n4 + 42 * n3 + 7 * n2;
    poly = poly - 122 * s_n;
    poly = poly - (seq_t)120;

    /* Cancel remaining 2s and 3s from poly */
    while (twos > 0 && (poly & 1) == 0) {
        poly >>= 1;
        twos--;
    }
    while (threes > 0 && poly % 3 == 0) {
        poly /= 3;
        threes--;
    }

    result *= poly;

    /* Any leftover factors of 2 or 3 (should be none) */
    while (twos > 0) {
        result /= 2;
        twos--;
    }
    while (threes > 0) {
        result /= 3;
        threes--;
    }

    return result;
}
