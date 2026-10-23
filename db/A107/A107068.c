/*
 * A107068  Expansion of 1/((1+x)^3-x^4).
 * https://oeis.org/A107068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 35, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = -3*a(n-1)-3*a(n-2)-a(n-3)+a(n-4),  a(0)=1, a(1)=-3, a(2)=6, a(3)=-10 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return -3;
    if (n == 2) return 6;
    if (n == 3) return -10;

    seq_t prev4 = 1;   /* a(0) */
    seq_t prev3 = -3;  /* a(1) */
    seq_t prev2 = 6;   /* a(2) */
    seq_t prev1 = -10; /* a(3) */
    seq_t curr = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* curr = -3*prev1 - 3*prev2 - prev3 + prev4 */
        curr = -((seq_t)3 * prev1) - ((seq_t)3 * prev2) - prev3 + prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
