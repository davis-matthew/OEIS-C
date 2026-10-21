/*
 * A206604  Number of integers in the smallest interval containing both
 *          minimal and maximal possible apex values of an addition triangle
 *          whose base is a permutation of n+1 consecutive integers.
 * https://oeis.org/A206604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=0..n} C(n,⌊k/2⌋) * (2*k - n)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* maximum value of ⌊k/2⌋ needed is ⌊n/2⌋ */
    idx_t max_r = n / 2;
    seq_t *C = (seq_t*)malloc((max_r + 1) * sizeof(seq_t));
    if (!C) return 0; /* unlikely, but guard against OOM */

    /* compute binomial coefficients C(n,r) for 0 <= r <= max_r */
    C[0] = 1;
    for (idx_t r = 1; r <= max_r; ++r) {
        /* C(n,r) = C(n,r-1) * (n - r + 1) / r */
        C[r] = C[r - 1] * (seq_t)(n - r + 1) / (seq_t)r;
    }

    seq_t pos = 0, neg = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t f   = k / 2;                     /* floor(k/2) */
        seq_t coeff = C[f];
        idx_t diff = (k * 2 > n) ? (k * 2 - n) : (n - k * 2);
        if (k * 2 > n) {
            pos += coeff * (seq_t)diff;
        } else {
            neg += coeff * (seq_t)diff;
        }
    }

    free(C);
    /* pos should be >= neg; the final value is (pos - neg) + 1 */
    return pos - neg + 1;
}
