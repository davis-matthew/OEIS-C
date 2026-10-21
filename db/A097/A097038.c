/*
 * A097038  A Jacobsthal variant.
 * https://oeis.org/A097038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(n+1) - 1)/3 , n odd * a(n) = (2^(n+1) + 1)/3 -
 *            2^(n/2) , n even
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute 2^exp using repeated multiplication by 2. */
/* This routine works for both integral and arbitrary‑precision seq_t. */
static seq_t pow2(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res = res * 2;
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t pow2_nplus1 = pow2(n + 1);

    if (n & 1U)          /* n is odd */
    {
        seq_t numerator = pow2_nplus1 - (seq_t)1;
        return numerator / (seq_t)3;
    }
    else                 /* n is even */
    {
        seq_t numerator = pow2_nplus1 + (seq_t)1;
        seq_t term1     = numerator / (seq_t)3;
        seq_t pow2_half = pow2(n / 2);
        return term1 - pow2_half;
    }
}
