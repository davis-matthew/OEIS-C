/*
 * A157093  Consider all Consecutive Integer Pythagorean 9-tuples
 *          (X,X+1,X+2,X+3,X+4,Z-3,Z-2,Z-1,Z) ordered by increasing Z;
 *          sequence gives Z values.
 * https://oeis.org/A157093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - a(n-2) - 24, a(0)=4, a(1)=44
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)44;

    /* Use a simple iterative recurrence to avoid any dynamic allocation
       and to keep it compatible with both fixed‑width and arbitrary‑precision
       seq_t types. */
    seq_t prev2 = (seq_t)4;          /* a(0) */
    seq_t prev1 = (seq_t)44;         /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (prev1 * (seq_t)18) - prev2 - (seq_t)24;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
