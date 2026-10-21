/*
 * A077959  Expansion of 1/(1+2*x^3).
 * https://oeis.org/A077959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if n mod 3 ≠ 0; a(n) = (-2)^(n/3) otherwise.
 *            Recurrence: a(0)=1, a(1)=a(2)=0, a(n)=−2*a(n−3)
 * Range:     i8 n <= 23, i16 n <= 47, i32 n <= 95, i64 n <= 191,
 *            i128 n <= 383, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Zero for indices not divisible by 3 */
    if (n % 3 != 0) {
        return (seq_t)0;
    }

    /* Compute (-2)^(n/3) via repeated multiplication. */
    idx_t k = n / 3;
    seq_t result = 1;
    for (idx_t i = 0; i < k; ++i) {
        result = -2 * result;
    }
    return result;
}
