/*
 * A192883  Constant term in the reduction by (x^2 -> x + 1) of the polynomial
 *          F(n+3)*x^n, where F = A000045 (Fibonacci sequence).
 * https://oeis.org/A192883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A192883: a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3), 
   a(0)=2, a(1)=0, a(2)=5. */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;

    /* Use recurrence: a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) */
    seq_t a0 = (seq_t)2;  /* a(0) */
    seq_t a1 = (seq_t)0;  /* a(1) */
    seq_t a2 = (seq_t)5;  /* a(2) */
    seq_t curr = a2;

    for (idx_t i = 3; i <= n; ++i) {
        /* curr = 2*a2 + 2*a1 - a0; implemented with additions only */
        curr = a2 + a2 + a1 + a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = curr;
    }
    return a2;
}
