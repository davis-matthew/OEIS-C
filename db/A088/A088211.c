/*
 * A088211  Denominators of convergents of the continued fraction with the n+1
 *          partial quotients: [2;2,2,...(n 2's)...,2,n+1], starting with [1],
 *          [2;2], [2;2,3], [2;2,2,4], ...
 * https://oeis.org/A088211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   a(n) = P_{n+1} + (n-1)*P_n (Pell numbers) Pell numbers: P_0 = 0,
 *            P_1 = 1, P_k = 2*P_{k-1} + P_{k-2} for k >= 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return 1;
    if (n == 1) return 2;

    /* Compute Pell numbers up to P_{n+1} */
    seq_t p_prev = 0;  /* P_0 */
    seq_t p_cur  = 1;  /* P_1 */
    idx_t i, limit = n + 1;   /* compute up to P_{n+1} */

    for (i = 2; i <= limit; ++i) {
        seq_t p_next = 2 * p_cur + p_prev;
        p_prev = p_cur;
        p_cur  = p_next;
    }

    /* Now p_prev = P_n, p_cur = P_{n+1} */
    return p_cur + ((seq_t)(n - 1)) * p_prev;
}
