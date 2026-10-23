/*
 * A135272  Row 8 of A038207.
 * https://oeis.org/A135272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, nonn, full
 * Formula:   a(n) = C(8,n) * 2^(8-n) for 0 ≤ n ≤ 8, else 0
 * Range:     u8 none, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n > 8 the row has no terms */
    if (n > (idx_t)8) {
        return (seq_t)0;
    }

    /* Compute the binomial coefficient C(8,n) */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* Multiply by (8 - i + 1) and divide by i.
           The division is exact because it is a binomial coefficient. */
        res = res * (seq_t)(8 - i + 1) / (seq_t)i;
    }

    /* Multiply by 2^(8-n) using a left shift */
    res <<= (int)(8 - n);
    return res;
}
