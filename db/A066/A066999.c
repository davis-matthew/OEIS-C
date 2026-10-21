/*
 * A066999  a(n) = 3^n * Sum_{i=1..n} i^3/3^i.
 * https://oeis.org/A066999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + n^3; a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* value below offset is irrelevant */
    seq_t value = (seq_t)1;       /* a(1) */
    if (n == 1)
        return value;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;      /* cast index to seq_t */
        value = 3 * value + ii * ii * ii;
    }
    return value;
}
