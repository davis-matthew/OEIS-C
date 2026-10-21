/*
 * A240353  Inverse of 68th cyclotomic polynomial.
 * https://oeis.org/A240353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the n‑th coefficient of the formal power series
 *            expansion of 1/Φ68(x), where Φ68(x) = 1 – x² + x⁴ – x⁶ + … +
 *            x³². The recurrence is: a(0) = 1 a(n) = –∑ₖ₌₁^{min(n,32)} φ[k] ·
 *            a(n–k) with φ[2m] = (m even ? +1 : –1), φ[odd] = 0. The sequence
 *            is periodic with period 68 (offset 0)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Period = 68 */
    static bool initialized = false;
    static seq_t seq[68] = {0};

    if (!initialized) {
        /* Φ68(x) coefficients: φ[even] = (k/2 even? +1 : –1), φ[odd] = 0 */
        seq[0] = 1;          /* a(0) = 1 */
        for (idx_t i = 1; i < 68; ++i) {
            seq_t sum = 0;
            /* sum over k = 2,4,… up to min(i,32) */
            for (idx_t k = 2; k <= i && k <= 32; k += 2) {
                idx_t m = k >> 1;        /* k = 2m   */
                int coeff = (m & 1) ? -1 : 1;  /* φ[2m] */
                sum += (seq_t)(coeff) * seq[i - k];
            }
            seq[i] = -sum;
        }
        initialized = true;
    }

    return seq[n % 68];
}
