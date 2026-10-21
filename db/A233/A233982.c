/*
 * A233982  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having the sum of the squares of the edge differences equal to 10,
 *          and no two adjacent values equal.
 * https://oeis.org/A233982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2) + 4*a(n-3) with a(1)=56, a(2)=236,
 *            a(3)=976
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                     /* behavior for n<1 irrelevant */

    if (n == 1)
        return (seq_t)56;
    if (n == 2)
        return (seq_t)236;
    if (n == 3)
        return (seq_t)976;

    /* iterate the recurrence using only the last three values */
    seq_t a0 = (seq_t)56;   /* a(1)   */
    seq_t a1 = (seq_t)236;  /* a(2)   */
    seq_t a2 = (seq_t)976;  /* a(3)   */
    seq_t an = a2;

    for (idx_t i = 4; i <= n; ++i) {
        an = a2 * (seq_t)2U + a1 * (seq_t)8U + a0 * (seq_t)4U;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return an;
}
