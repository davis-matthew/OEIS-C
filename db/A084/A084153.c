/*
 * A084153  Binomial transform of a Jacobsthal convolution.
 * https://oeis.org/A084153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5^n - 2*2^n + (-1)^n)/18
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=0, a(2)=1, a(n)=6*a(n-1)-3*a(n-2)-10*a(n-3) for n≥3 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;

    seq_t a0 = 0;   /* a(n-3) */
    seq_t a1 = 0;   /* a(n-2) */
    seq_t a2 = 1;   /* a(n-1) */
    seq_t a3;       /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        a3 = 6 * a2;
        a3 -= 3 * a1;
        a3 -= 10 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
