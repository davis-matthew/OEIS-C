/*
 * A090555  Erroneous version of A014573.
 * https://oeis.org/A090555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1) = 10; for n>=2, a(n) = 2^(n-2)
 * Range:     i8 n <= 6, i16 n <= 6, i32 n <= 6, i64 n <= 6, i128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)10;
    seq_t res = 1;
    for (idx_t i = 0; i < n - 2; ++i)
        res = res * 2;
    return res;
}
