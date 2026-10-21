/*
 * A052936  Expansion of (1-x)*(1-2*x)/(1-5*x+5*x^2).
 * https://oeis.org/A052936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=7; a(n) = 5*a(n-1) - 5*a(n-2) for n >= 3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)7;

    seq_t prev2 = (seq_t)2;          /* a(n-2) : a(1) */
    seq_t prev1 = (seq_t)7;          /* a(n-1) : a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t mul1 = (seq_t)5 * prev1;  /* 5 * a(n-1) */
        seq_t mul2 = (seq_t)5 * prev2;  /* 5 * a(n-2) */
        curr = mul1 - mul2;             /* a(n) = 5*a(n-1) - 5*a(n-2) */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
