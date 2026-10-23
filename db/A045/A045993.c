/*
 * A045993  Expansion of (1-x)/(1 - 10*x + 18*x^2 - 8*x^3).
 * https://oeis.org/A045993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=9, a(2)=72; a(n) = 10*a(n-1) - 18*a(n-2) + 8*a(n-3)
 *            for n >= 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three indices directly */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)9;
    if (n == (idx_t)2) return (seq_t)72;

    /* iterative computation using only three variables */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)9;   /* a(n-2) */
    seq_t a2 = (seq_t)72;  /* a(n-1) */

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        seq_t next = (seq_t)10 * a2 - (seq_t)18 * a1 + (seq_t)8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
