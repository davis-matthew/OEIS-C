/*
 * A105940  a(n) = binomial(n+5, 5)*binomial(n+8, 5).
 * https://oeis.org/A105940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+5,5) * binomial(n+8,5)
 * Range:     u8 none, u16 n <= 3, u32 n <= 19, u64 n <= 215, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute binomial(n+5,5) = (n+5)(n+4)(n+3)(n+2)(n+1)/120 */
    seq_t n1 = (seq_t)(n + 1);
    seq_t n2 = (seq_t)(n + 2);
    seq_t n3 = (seq_t)(n + 3);
    seq_t n4 = (seq_t)(n + 4);
    seq_t n5 = (seq_t)(n + 5);

    seq_t bin1 = n5;
    bin1 = bin1 * n4;
    bin1 = bin1 * n3;
    bin1 = bin1 * n2;
    bin1 = bin1 * n1;
    bin1 = bin1 / 120; /* 5! */

    /* Compute binomial(n+8,5) = (n+8)(n+7)(n+6)(n+5)(n+4)/120 */
    seq_t n6 = (seq_t)(n + 6);
    seq_t n7 = (seq_t)(n + 7);
    seq_t n8 = (seq_t)(n + 8);

    seq_t bin2 = n8;
    bin2 = bin2 * n7;
    bin2 = bin2 * n6;
    bin2 = bin2 * n5;
    bin2 = bin2 * n4;
    bin2 = bin2 / 120; /* 5! */

    return bin1 * bin2;
}
