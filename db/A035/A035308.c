/*
 * A035308  Expansion of 1/(1-100*x)^(1/10), related to deca-factorial numbers
 *          A045757.
 * https://oeis.org/A035308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^n * Π_{j=1..n} (10*j-9) / n! (n ≥ 1), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t factor = (seq_t)(10 * i - 9);
        res = res * factor * (seq_t)10 / i;
    }
    return res;
}
