/*
 * A169471  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0, otherwise 26 * 25^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* compute 25^(n-1) by repeated squaring */
    idx_t e = n - 1;
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)25;
    while (e) {
        if (e & 1) result = result * base;
        base = base * base;
        e >>= 1;
    }

    return (seq_t)26 * result;
}
