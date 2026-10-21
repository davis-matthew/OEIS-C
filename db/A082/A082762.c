/*
 * A082762  Trinomial transform of Lucas numbers (A000032).
 * https://oeis.org/A082762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2), a(0)=1, a(1)=8
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t a0 = (seq_t)1;   /* a(n-2)  */
    seq_t a1 = (seq_t)8;   /* a(n-1)  */
    seq_t a2;              /* a(n)    */

    for (idx_t i = 2; i <= n; ++i) {
        a2 = 6 * a1 - 4 * a0;  /* recurrence step */
        a0 = a1;
        a1 = a2;
    }
    return a1;   /* a(n) after loop */
}
