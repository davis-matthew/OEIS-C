/*
 * A237357  The number of tilings of the 3 X 3 X (2n) room with 1 X 2 X 3
 *          boxes.
 * https://oeis.org/A237357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 22*a(n-2) + 36*a(n-3), with a(0)=1, a(1)=6,
 *            a(2)=64
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

extern seq_t a(idx_t n)
{
    /* handle the first three terms explicitly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;
    if (n == 2)
        return (seq_t)64;

    /* iterate using the recurrence, keeping only the last three values */
    seq_t a0 = (seq_t)1;   /* a(i-3)  */
    seq_t a1 = (seq_t)6;   /* a(i-2)  */
    seq_t a2 = (seq_t)64;  /* a(i-1)  */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t val = a2 * (seq_t)7 + a1 * (seq_t)22 + a0 * (seq_t)36;
        a0 = a1;
        a1 = a2;
        a2 = val;
    }

    return a2;
}
