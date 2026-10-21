/*
 * A092974  Row 9 of array in A288580.
 * https://oeis.org/A092974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Product_{i=0..⌊2n/9⌋} (n - 9·i), skipping the zero factor
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 35, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t max_i = (2 * n) / 9;
    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t term = (seq_t)n - (seq_t)9 * (seq_t)i;
        if (term != 0) {
            result *= term;
        }
    }
    return result;
}
