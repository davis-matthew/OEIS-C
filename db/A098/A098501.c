/*
 * A098501  Number of squares on infinite octant of chessboard at <=n knight
 *          moves from the corner. The octant includes the diagonal.
 * https://oeis.org/A098501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1,2,5,13,31 for 0 ≤ n ≤ 4
 *            a(n) = (14*n^2 + 8*n + 5 + 3*(-1)^n) / 8   for n > 4
 */
seq_t a(idx_t n)
{
    /* Small n: use the given constant terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)13;
    if (n == 4) return (seq_t)31;

    /* n > 4: evaluate the closed‑form expression */
    seq_t term = ((seq_t)14 * n) * n;   /* 14 * n^2 */
    term += ((seq_t)8 * n);             /* + 8*n */
    term += (seq_t)5;                   /* + 5 */

    /* add 3 if n is even, subtract 3 if n is odd */
    if ((n & (idx_t)1ULL) == 0)
        term += (seq_t)3;
    else
        term -= (seq_t)3;

    /* divide by 8 */
    return term / (seq_t)8;
}
