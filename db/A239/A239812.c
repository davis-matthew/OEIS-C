/*
 * A239812  Number of n X 1 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it, modulo 4.
 * https://oeis.org/A239812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=5, a(3)=11, a(n)=a(n-1)+2*a(n-2)+2*a(n-3) for n≥4 */

seq_t a(idx_t n)
{
    /* handle base cases directly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;

    /* iterate the recurrence from 4 to n */
    seq_t a1 = (seq_t)2;   /* a(1) */
    seq_t a2 = (seq_t)5;   /* a(2) */
    seq_t a3 = (seq_t)11;  /* a(3) */
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = a3 + ((seq_t)2) * a2 + ((seq_t)2) * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
