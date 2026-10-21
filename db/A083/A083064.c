/*
 * A083064  Square number array T(n,k) = (k*(k+2)^n+1)/(k+1) read by
 *          antidiagonals.
 * https://oeis.org/A083064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = value obtained by reading the infinite array * T(n,k) =
 *            (k*(k+2)^n + 1) / (k+1) by antidiagonals. * The array is indexed
 *            with n = row, k = column (both ≥ 0)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Helper: binary exponentiation for seq_t values. */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Find the diagonal index m such that
     * n lies in the diagonal with i + j = m.
     * The count of elements up to diagonal m-1 is m*(m+1)/2.
     */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n) {
        m++;
    }

    /* Number of terms before diagonal m */
    idx_t prev_cum = m * (m + 1) / 2;
    /* Offset within the diagonal (0 … m) */
    idx_t offset = n - prev_cum;
    idx_t row = offset;          /* exponent */
    idx_t col = m - offset;      /* base index */

    /* Compute T(row, col) = (col * (col+2)^row + 1) / (col+1) */
    seq_t base = (seq_t)(col + 2);
    seq_t pow_val = pow_seq(base, row);
    seq_t numerator = (seq_t)col * pow_val + 1;
    seq_t denominator = (seq_t)(col + 1);

    return numerator / denominator;  /* exact division */
}
