/*
 * A102773  a(n) = Sum_{i=0..n} binomial(n,i)^2*i!*4^i.
 * https://oeis.org/A102773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=5,
   a(n) = (8*n-3)*a(n-1) - 16*(n-1)^2*a(n-2)  for n≥2
   (This follows from the OEIS definition). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t prev2 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)5;          /* a(1) */
    seq_t cur   = prev1;

    for (idx_t i = 2; i <= n; ++i) {
        /* (8*n-3)*prev1  */
        seq_t mult1 = ((seq_t)8 * (seq_t)i - (seq_t)3) * prev1;

        /* 16*(n-1)^2*prev2  */
        seq_t t = (seq_t)(i - 1);
        seq_t mult2 = (seq_t)16 * t * t * prev2;

        cur = mult1 - mult2;

        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
