/*
 * A082308  Expansion of e.g.f. (1+x)*exp(4*x)*cosh(x).
 * https://oeis.org/A082308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+3)*3^(n-1) + (n+5)*5^(n-1))/2, a(0)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;          /* a(0) = 1 */

    seq_t nseq = (seq_t)n;
    seq_t pow3 = pow_seq((seq_t)3, n - 1);
    seq_t pow5 = pow_seq((seq_t)5, n - 1);

    seq_t term1 = (nseq + (seq_t)3) * pow3;
    seq_t term2 = (nseq + (seq_t)5) * pow5;

    return (term1 + term2) / 2;
}
