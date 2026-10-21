/*
 * A233868  Numbers that are not the sum of two evil numbers.
 * https://oeis.org/A233868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=4, a(4)=7, a(5)=31; a(n)=5*a(n-1)-4*a(n-2)
 *            for n>5 (OEIS recurrence)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined, but harmless */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)7;
    if (n == 5) return (seq_t)31;

    seq_t prev2 = (seq_t)7;   /* a(4) */
    seq_t prev1 = (seq_t)31;  /* a(5) */
    seq_t cur;

    for (idx_t i = 6; i <= n; ++i) {
        cur = (seq_t)5 * prev1 - (seq_t)4 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
