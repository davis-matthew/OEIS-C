/*
 * A051754  Consider problem of placing N queens on an n X n board so that
 *          each queen attacks precisely 1 other. Sequence gives maximum
 *          number of queens.
 * https://oeis.org/A051754
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 2*floor(2*n/3) for n ≥ 6, with special cases a(2) = 2,
 *            a(3) = 2, a(4) = 4, a(5) = 4. This matches the OEIS A051754
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2 || n == 3)
        return (seq_t)2;
    if (n == 4 || n == 5)
        return (seq_t)4;

    /* n ≥ 6 */
    seq_t nn   = (seq_t)n;          /* safe cast */
    seq_t two_n = nn + nn;          /* 2*n   */
    seq_t q     = two_n / (seq_t)3; /* floor(2*n / 3) */
    return q * (seq_t)2;            /* 2*floor(2*n / 3) */
}
