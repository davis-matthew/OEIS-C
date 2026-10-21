/*
 * A095126  Expansion of x*(4+5*x-x^2)/ (1-2*x-3*x^2+x^3).
 * https://oeis.org/A095126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - a(n-3) for n ≥ 4 a(1) = 4, a(2) =
 *            13, a(3) = 37 (coefficients derived from the rational generating
 *            function x*(4+5*x-x^2)/(1-2*x-3*x^2+x^3))
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms explicitly */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)37;

    /* Use a small sliding window to avoid large memory usage */
    seq_t a_nm3 = (seq_t)4;  /* a(n-3) */
    seq_t a_nm2 = (seq_t)13; /* a(n-2) */
    seq_t a_nm1 = (seq_t)37; /* a(n-1) */
    seq_t a_n;               /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = a_nm1 * (seq_t)2 + a_nm2 * (seq_t)3 - a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
