/*
 * A002800  Erroneous version of A001157.
 * https://oeis.org/A002800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = sigma_2(n) = Sum_{d|n} d^2 (A001157), except the
 *            misprinted term a(6) = 53 that this dead sequence records
 *            instead of 50
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 18, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;

    if (n == 6)
        return 53; /* the erroneous term this sequence exists to record */

    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            result += (seq_t)d * (seq_t)d;
            if (d * d != n) {
                idx_t other = n / d;
                result += (seq_t)other * (seq_t)other;
            }
        }
    }

    return result;
}
