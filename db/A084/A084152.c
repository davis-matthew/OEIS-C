/*
 * A084152  Exponential self-convolution of Jacobsthal numbers (divided by 2).
 * https://oeis.org/A084152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4^n - 2 + (-2)^n)/18 Recurrence: a(n) = 3*a(n-1) +
 *            6*a(n-2) - 8*a(n-3), a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;

    seq_t a0 = (seq_t)0;  /* a(n-3) */
    seq_t a1 = (seq_t)0;  /* a(n-2) */
    seq_t a2 = (seq_t)1;  /* a(n-1) */
    seq_t an = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        /* an = 3*a2 + 6*a1 - 8*a0; all terms non‑negative. */
        an = (seq_t)(3 * a2 + 6 * a1) - (seq_t)(8 * a0);
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
