/*
 * A098602  a(n) = A001652(n) * A046090(n).
 * https://oeis.org/A098602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A001653(n)^2 - 1)/2 where A001653(1)=5, A001653(2)=29
 *            and A001653(n)=6*A001653(n-1)-A001653(n-2) for n≥3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)420;

    seq_t s_prev2 = (seq_t)5;   /* A001653(1) */
    seq_t s_prev1 = (seq_t)29;  /* A001653(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t s = (seq_t)6 * s_prev1 - s_prev2;
        s_prev2 = s_prev1;
        s_prev1 = s;
    }

    seq_t res = (s_prev1 * s_prev1 - (seq_t)1) / (seq_t)2;
    return res;
}
