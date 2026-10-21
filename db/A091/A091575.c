/*
 * A091575  Poincaré series [or Poincare series] of the preprojective algebra
 *          of a Dynkin diagram of type E_8.
 * https://oeis.org/A091575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*(n+1) + 8*max(0, n-29) -
 *            2*∑_{e∈{1,7,11,13,17,19,23,29}, e≤n} (n-e+1)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t RANK = 8;
    const idx_t H     = 30;                   /* Coxeter number for E8 */
    const idx_t E[8] = {1, 7, 11, 13, 17, 19, 23, 29};

    /* Term from n in the numerator (constant rank 8) */
    seq_t ans = RANK * (((seq_t)n) + 1);

    /* Term from n*x^h in the numerator */
    if (n >= H) {
        ans += RANK * (((seq_t)n) - H + 1);   /* n - h + 1 = n - 29 */
    }

    /* Subtractive terms from the exponents */
    for (idx_t i = 0; i < 8; ++i) {
        if (E[i] <= n) {
            ans -= 2 * (((seq_t)n) - E[i] + 1);
        }
    }

    return ans;
}
