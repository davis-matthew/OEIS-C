/*
 * A079351  a(1)=3; for n > 1, a(n) is the smallest integer greater than
 *          a(n-1) consistent with the condition "n is in the sequence if and
 *          only if a(n) is congruent to 0 (mod 5)".
 * https://oeis.org/A079351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(3*5^k + j) = 5^(k+1) + 3j + 2|j|, where k ≥ 0 and -2⋅5^k ≤ j <
 *            2⋅5^k. Equivalent: a(a(n)) = 5n. This function implements the
 *            closed‑form formula
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for int64_t */

static seq_t pow5(int exponent) {
    seq_t result = 1;
    for (int i = 0; i < exponent; ++i) {
        result *= (seq_t)5;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* out of range – undefined behaviour otherwise */

    /* Find k such that 5^k ≤ n < 5^(k+1).  Use division to avoid overflow. */
    idx_t pow = 1;      /* pow = 5^k */
    int k = 0;
    while (pow <= n / 5) {
        pow *= 5;
        ++k;
    }

    /* j = n – 3⋅5^k may be negative */
    int64_t j = (int64_t)n - 3 * (int64_t)pow;

    seq_t pow5k1 = pow5(k + 1);   /* 5^(k+1) */

    if (j >= 0) {
        /* a(n) = 5^(k+1) + 5·j  (since j ≥ 0) */
        return pow5k1 + (seq_t)5 * (seq_t)j;
    } else {
        /* a(n) = 5^(k+1) – |j|  (since j < 0) */
        seq_t abs_j = (seq_t)(-j);
        return pow5k1 - abs_j;
    }
}
