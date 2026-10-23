/*
 * A103975  Smaller side in (a,a+1,a+1)-integer triangle with integer area.
 * https://oeis.org/A103975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 15*a(n-2) + a(n-3) with initial values
 *            a(1)=16, a(2)=240, a(3)=3360 (recurrence holds for n >= 4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)240;
    if (n == 3) return (seq_t)3360;

    /* Iterative computation using only the last three terms */
    seq_t a1 = (seq_t)16;   /* a(n-3) */
    seq_t a2 = (seq_t)240;  /* a(n-2) */
    seq_t a3 = (seq_t)3360; /* a(n-1) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 * (seq_t)15 - a2 * (seq_t)15 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
