/*
 * A232584  Number of (n+1)X(3+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally or
 *          antidiagonally, with no adjacent elements equal.
 * https://oeis.org/A232584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3), a(1)=2, a(2)=12, a(3)=42,
 *            offset = 1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n=0, but safe to return 0 */

    /* Handle the first three terms explicitly */
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)12;
    if (n == 3)
        return (seq_t)42;

    /* Iteratively apply the recurrence:
       a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3) */
    seq_t a1 = (seq_t)2;   /* a(n-3)   */
    seq_t a2 = (seq_t)12;  /* a(n-2)   */
    seq_t a3 = (seq_t)42;  /* a(n-1)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)4 * a3 - (seq_t)4 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
