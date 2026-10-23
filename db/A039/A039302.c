/*
 * A039302  Number of distinct quadratic residues mod 5^n.
 * https://oeis.org/A039302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^(n+1) + 9 - 2*(-1)^n)/12 Recurrence: a(n) = 5*a(n-1) +
 *            a(n-2) - 5*a(n-3) Initial values: a(0)=1, a(1)=3, a(2)=11 This
 *            implementation uses the recurrence to avoid overflow in
 *            intermediate 5^n calculations. It runs in O(n) time and is valid
 *            for all indices n ≥ 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)3;
    if (n == 2U) return (seq_t)11;

    seq_t a0 = 1;           /* a(n-3) */
    seq_t a1 = 3;           /* a(n-2) */
    seq_t a2 = 11;          /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        /* an = 5*a2 + a1 - 5*a0  →  an = 5*(a2 - a0) + a1  (no underflow) */
        an = (a2 - a0);
        an *= (seq_t)5;
        an += a1;

        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2;
}
