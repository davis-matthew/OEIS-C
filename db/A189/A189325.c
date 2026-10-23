/*
 * A189325  Number of nondecreasing arrangements of n+2 numbers in 0..8 with
 *          the last equal to 8 and each after the second equal to the sum of
 *          one or two of the preceding four.
 * https://oeis.org/A189325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{13} c_k * C(n‑k+2, 3) with coefficients c = {13,
 *            -28, 31, -9, 10, 3, 7, -21, -14, -10, 2, 10, 7, 1}. C(m,3) is
 *            the binomial coefficient (m choose 3), taken as 0 for m < 3
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: binomial coefficient (m choose 3) for m >= 3, 0 otherwise. */
static inline seq_t binom3(idx_t m)
{
    if (m < 3) return 0;
    unsigned long long mm = m;
    unsigned long long prod = mm * (mm - 1) * (mm - 2);
    prod /= 6;                    /* division is exact */
    return (seq_t)prod;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Coefficients of the numerator polynomial of the generating function */
    const int64_t coeff[14] = {
        13, -28, 31, -9, 10, 3, 7, -21, -14, -10, 2, 10, 7, 1
    };

    int64_t sum = 0;              /* signed accumulator */
    for (int k = 0; k < 14; ++k) {
        int64_t c = coeff[k];
        if (c == 0) continue;
        /* For a(n), terms with n‑k+2 < 3 contribute nothing */
        if (n < (idx_t)k) continue;           /* then n-k+2 < 2 < 3 */
        idx_t m = n - k + 2;
        if (m < 3) continue;
        seq_t bin = binom3(m);
        sum += c * (int64_t)bin;
    }

    /* The result is guaranteed non‑negative for n ≥ 1. */
    return (seq_t)sum;
}
