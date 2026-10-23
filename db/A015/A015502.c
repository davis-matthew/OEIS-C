/*
 * A015502  a(1) = 1, a(n) = Sum_{k=1..n-1} (3^k - 1)/2 * a(k).
 * https://oeis.org/A015502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=1, a(n)=((3^(n-1)+1)/2)*a(n-1) for n≥3
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) /* offset is 1; behaviour for n<1 is irrelevant */
        return 0;
    if (n == 1 || n == 2)   /* first two terms are 1 */
        return 1;

    seq_t a_val = 1;       /* a(2) = 1 to start the recurrence */
    seq_t p = 9;           /* p = 3^(3-1) = 3^2 for the first recurrence step */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t factor = (p + 1) / 2;   /* ((3^(i-1)+1)/2) */
        a_val = a_val * factor;       /* update a(i) */
        p = p * 3;                    /* advance p to 3^(i) for next step */
    }
    return a_val;
}
