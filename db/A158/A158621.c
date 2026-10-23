/*
 * A158621  Partial products of A001093.
 * https://oeis.org/A158621
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = PROD_{k=2..n} (k^3 + 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 2; behaviour below the offset is irrelevant. */
    if (n < 2) {
        return 0;
    }
    seq_t result = 1;
    for (idx_t k = 2; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t term = kk * kk * kk + 1;
        result *= term;
    }
    return result;
}
