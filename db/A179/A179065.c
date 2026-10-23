/*
 * A179065  Number of non-attacking placements of 10 rooks on an n X n board.
 * https://oeis.org/A179065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10! * binomial(n,10)^2
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 11, u64 n <= 24, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10)
        return 0;

    const seq_t FACT10 = (seq_t)3628800;   /* 10! */

    seq_t binom = 1;
    for (idx_t i = 1; i <= 10; ++i) {
        binom = binom * (seq_t)(n - i + 1);
        binom = binom / (seq_t)i;
    }

    seq_t result = FACT10 * binom;
    result = result * binom;
    return result;
}
