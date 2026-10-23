/*
 * A017382  a(n) = (10*n + 9)^6.
 * https://oeis.org/A017382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 9)^6
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 161, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type to avoid overflow */
    seq_t t = (seq_t)n;

    /* Compute the base value 10*n + 9 */
    seq_t base = ((seq_t)10 * t) + (seq_t)9;

    /* Compute base^6 via repeated multiplication */
    seq_t res = base;
    for (int i = 0; i < 5; ++i) {
        res *= base;
    }

    return res;
}
