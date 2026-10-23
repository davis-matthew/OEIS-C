/*
 * A110328  Row sums of a number triangle related to the Pell numbers.
 * https://oeis.org/A110328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3; for n≥2: a(n)=2*n*a(n‑1)+n*(n‑1)*a(n‑2)+1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t ii = (seq_t)i;                /* convert loop variable to seq_t */
        curr = (seq_t)2 * ii * prev1;        /* 2*i*a(n-1) */
        curr = curr + ii * (ii - (seq_t)1) * prev2;  /* + i*(i-1)*a(n-2) */
        curr = curr + (seq_t)1;             /* + 1 */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
