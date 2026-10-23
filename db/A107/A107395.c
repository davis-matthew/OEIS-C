/*
 * A107395  a(n) = binomial(n+4,4)*binomial(n+6,4).
 * https://oeis.org/A107395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+4,4)*binomial(n+6,4)
 * Range:     u8 none, u16 n <= 5, u32 n <= 31, u64 n <= 563, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;

    /* compute binomial(tn+4,4) = (tn+1)(tn+2)(tn+3)(tn+4)/24 */
    seq_t bin1 = tn + 1;
    bin1 *= (tn + 2);
    bin1 *= (tn + 3);
    bin1 *= (tn + 4);
    bin1 /= 24;

    /* compute binomial(tn+6,4) = (tn+3)(tn+4)(tn+5)(tn+6)/24 */
    seq_t bin2 = tn + 3;
    bin2 *= (tn + 4);
    bin2 *= (tn + 5);
    bin2 *= (tn + 6);
    bin2 /= 24;

    return bin1 * bin2;
}
