/*
 * A088859  a(n) = L(n) + 2^n where L(n) = A000032(n) (the Lucas numbers).
 * https://oeis.org/A088859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = L(n) + 2^n, where L(n) are Lucas numbers
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the sequence: a(0)=3, a(1)=3, a(2)=7, */
/* a(n)=3*a(n-1)-a(n-2)-2*a(n-3) for n≥3. */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;

    /* Use the recurrence to generate values up to n. */
    seq_t a0 = (seq_t)3;   /* a(n-3)  */
    seq_t a1 = (seq_t)3;   /* a(n-2)  */
    seq_t a2 = (seq_t)7;   /* a(n-1)  */
    seq_t an = 0;

    idx_t i;
    for (i = 3; i <= n; ++i) {
        an = (seq_t)3 * a2 - a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
