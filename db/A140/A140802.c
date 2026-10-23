/*
 * A140802  a(n) = binomial(n+3, 3)*8^n.
 * https://oeis.org/A140802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,3)*8^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute C(n+3,3) = ((n+3)*(n+2)*(n+1))/6 */
    seq_t bin = ((seq_t)(n + 3)) * (seq_t)(n + 2);
    bin = bin * (seq_t)(n + 1);
    bin = bin / (seq_t)6;

    /* Compute 8^n */
    seq_t pow8 = 1;
    seq_t eight = (seq_t)8;
    idx_t i = 0;
    while (i < n) {
        pow8 *= eight;
        i++;
    }

    return bin * pow8;
}
