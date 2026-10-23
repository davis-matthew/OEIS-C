/*
 * A134068  a(n) = 2*a(n-2) + 4*a(n-3), with initial terms 0, 3, 3.
 * https://oeis.org/A134068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + 4*a(n-3), a(0)=0, a(1)=3, a(2)=3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1 || n == 2) return (seq_t)3;

    /* Keep only the last three values in order to avoid dynamic allocation */
    seq_t a0 = (seq_t)0;   /* a(i-3)  */
    seq_t a1 = (seq_t)3;   /* a(i-2)  */
    seq_t a2 = (seq_t)3;   /* a(i-1)  */
    seq_t an;              /* a(i)    */

    /* Iterate from i = 3 up to n, updating the three last terms each step */
    for (idx_t i = 3; i <= n; ++i) {
        an = a1 * (seq_t)2 + a0 * (seq_t)4;  /* a(i) = 2*a(i-2) + 4*a(i-3) */
        a0 = a1;          /* shift: a(i-3) becomes previous a(i-2) */
        a1 = a2;          /* shift: a(i-2) becomes previous a(i-1) */
        a2 = an;          /* shift: a(i-1) becomes current a(i)   */
    }

    return a2;  /* a(n) stored in a2 after the loop */
}
