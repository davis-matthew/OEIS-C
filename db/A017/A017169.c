/*
 * A017169  a(n) = (9*n)^9.
 * https://oeis.org/A017169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 15, u128 n <= 2123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (9*n) once, in the target type */
    seq_t base = (seq_t)9 * (seq_t)n;

    /* For n==0 the product is 0; for n>0 we must raise base to the 9th power */
    seq_t result = base;
    for (idx_t i = 1; i < 9; ++i) {
        result *= base;
    }
    return result;
}
