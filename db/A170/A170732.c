/*
 * A170732  Expansion of g.f.: (1+x)/(1 - 12*x).
 * https://oeis.org/A170732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 13 * 12^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    seq_t result = 1;
    /* Multiply 12 (n-1) times */
    for (idx_t i = 1; i < n; ++i) {
        result *= (seq_t)12;
    }
    result *= (seq_t)13;
    return result;
}
