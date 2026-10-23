/*
 * A173175  a(n) = sinh^2( 2n*arcsinh(sqrt n)).
 * https://oeis.org/A173175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sinh^2( 2n*arcsinh(sqrt n)) = (T_{2n}(2n+1)-1)/2 * where
 *            T_k(x) is the Chebyshev polynomial of the first kind. * This
 *            implementation uses the recurrence T_0=1, T_1=x,
 *            T_k=2*x*T_{k-1}-T_{k-2} * and the identity T_{2m}=2*T_m^2-1 to
 *            reduce the number of multiplications
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  p = 2*n + 1  (argument of the Chebyshev polynomial) */
    seq_t p = (seq_t)n * 2 + 1;

    /* Compute T_n(p) by the three‑term recurrence. */
    seq_t t_prev_prev = 1;   /* T_0(p) */
    seq_t t_prev     = p;    /* T_1(p) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t t_curr = 2 * p * t_prev - t_prev_prev;
        t_prev_prev = t_prev;
        t_prev = t_curr;
    }

    /* Now t_prev == T_n(p). Compute T_{2n} = 2*T_n^2 - 1. */
    seq_t t_2n = 2 * t_prev * t_prev - 1;

    /* Finally a(n) = (T_{2n} - 1) / 2.  The result is an integer. */
    return (t_2n - 1) / 2;
}
