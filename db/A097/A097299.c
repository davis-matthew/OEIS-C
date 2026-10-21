/*
 * A097299  Ninth column (m=8) of (1,6)-Pascal triangle A096956.
 * https://oeis.org/A097299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+48)*binomial(n+7,7)/8
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 52, u64 n <= 954,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* intermediate values use unsigned __int128 so that
       64‑bit builds can handle the multiplications without overflow. */
    __uint128_t binom = 1;
    __uint128_t factor;
    const idx_t k = 7;

    /* compute binomial coefficient binom(n+7,7) */
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by numerator (n+7-i+1) */
        factor = binom * ( __uint128_t )(n + 7 - (i - 1));
        /* divide exactly by i */
        binom = factor / i;
    }

    /* compute (n+48)*binom/8 */
    __uint128_t result = binom * ( __uint128_t )(n + 48);
    result /= 8;

    return (seq_t)result;
}
