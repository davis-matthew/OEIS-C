/*
 * A016944  a(n) = (6*n + 2)^12.
 * https://oeis.org/A016944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 2)^12
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 6, u128 n <= 270,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Compute (6*n + 2) once as seq_t.
    seq_t base = (seq_t)6 * (seq_t)n + (seq_t)2;
    // Raise base to the 12th power by repeated multiplication.
    seq_t result = (seq_t)1;
    for (unsigned i = 0; i < 12; ++i) {
        result *= base;
    }
    return result;
}
