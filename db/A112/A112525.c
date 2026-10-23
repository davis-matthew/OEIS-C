/*
 * A112525  Expansion of 1/(1 - 100*x^2 - 100*x^3).
 * https://oeis.org/A112525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 100*a(n-2) + 100*a(n-3) for n ≥ 3, a(0)=1, a(1)=0,
 *            a(2)=100
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)0;
    if (n == 2U) return (seq_t)100;

    seq_t a_n_minus_3 = (seq_t)1;   /* a(0) */
    seq_t a_n_minus_2 = (seq_t)0;   /* a(1) */
    seq_t a_n_minus_1 = (seq_t)100; /* a(2) */

    for (idx_t i = 3U; i <= n; ++i) {
        seq_t a_n = (seq_t)100 * a_n_minus_3 + (seq_t)100 * a_n_minus_2;
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }
    return a_n_minus_1;
}
