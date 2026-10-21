/*
 * A096656  a(n) = F(n+2)*a(n-1) + F(n+1)*a(n-2), where F = A000045 (Fibonacci
 *          numbers), a(0)=1, a(1)=2.
 * https://oeis.org/A096656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n+2)*a(n-1) + F(n+1)*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_im2 = (seq_t)1; /* a(n-2) */
    seq_t a_im1 = (seq_t)2; /* a(n-1) */

    /* Fibonacci state: f_prev = F(i), f_curr = F(i+1) */
    seq_t f_prev = (seq_t)1; /* F(1) */
    seq_t f_curr = (seq_t)1; /* F(2) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t f_ip1 = f_prev + f_curr;      /* F(i+1) */
        seq_t f_ip2 = f_ip1 + f_curr;        /* F(i+2) */

        seq_t ai = f_ip2 * a_im1 + f_ip1 * a_im2;

        f_prev = f_curr;
        f_curr = f_ip1;

        a_im2 = a_im1;
        a_im1 = ai;
    }

    return a_im1;
}
