/*
 * A025863  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^11)).
 * https://oeis.org/A025863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2+20*n)/440 + ((3*n^2+5*n+5) mod 11)/11 +
 *            ((3*n^2+3) mod 5)/5)
 * Range:     u8 n <= 0, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t coeff      = n2 + (seq_t)20 * N;
    seq_t termA      = coeff / (seq_t)440;
    seq_t remA       = coeff % (seq_t)440;
    seq_t b          = ((seq_t)3 * n2 + (seq_t)5 * N + (seq_t)5) % (seq_t)11;
    seq_t c          = ((seq_t)3 * n2 + (seq_t)3) % (seq_t)5;
    seq_t rem_total  = remA + b * (seq_t)40 + c * (seq_t)88;
    seq_t add        = rem_total / (seq_t)440;
    return termA + add;
}
