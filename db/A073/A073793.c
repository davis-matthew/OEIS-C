/*
 * A073793  Replace 6^k with (-6)^k in base 6 expansion of n.
 * https://oeis.org/A073793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0..5) = n; a(6*k + m) = -6*a(k) + m   (0 ≤ m < 6). */
seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t pow = 1;
    while (n > 0) {
        idx_t digit = n % 6;
        result += (seq_t)digit * pow;
        n /= 6;
        pow *= (seq_t)-6;
    }
    return result;
}
