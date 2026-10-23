/*
 * A199336  x-values in the solution to 15*x^2 - 14 = y^2.
 * https://oeis.org/A199336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = 8*a(n+2) - a(n), a(1)=1, a(2)=3, a(3)=5, a(4)=23
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return a(n) for n >= 1.  For n <= 4 we return the
       predefined values, otherwise compute by the linear recurrence. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)23;

    /* a0, a1, a2, a3 hold the four most recent values:
       a0   = a(k-3), a1 = a(k-2), a2 = a(k-1), a3 = a(k).  */
    seq_t a0 = (seq_t)1;   /* a(1) */
    seq_t a1 = (seq_t)3;   /* a(2) */
    seq_t a2 = (seq_t)5;   /* a(3) */
    seq_t a3 = (seq_t)23;  /* a(4) */

    for (idx_t k = 5; k <= n; ++k) {
        seq_t next = ((seq_t)8) * a2 - a0;  /* a(k) = 8*a(k-2) - a(k-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
