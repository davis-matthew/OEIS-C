/*
 * A087628  Generalized Jacobsthal sequence.
 * https://oeis.org/A087628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n, k(n-k)≡0 (mod 3)} C(n,k) The closed form: if
 *            n≡0 (mod 3) then a(n) = (2^n + 2·(-1)^n) / 3 otherwise a(n) =
 *            (2^(n+1) – 2·(-1)^n) / 3 which is an integer for all n≥0
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_seq(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (exp)
    {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 */
    if (n == 0) return (seq_t)1;

    if (n % (idx_t)3 == 0)
    {
        seq_t pow = pow2_seq(n);           /* 2^n   */
        if ((n & (idx_t)1) == 0)           /* n even */
            return (pow + (seq_t)2) / (idx_t)3;
        else                               /* n odd  */
            return (pow - (seq_t)2) / (idx_t)3;
    }
    else
    {
        seq_t pow = pow2_seq(n + 1);       /* 2^(n+1) */
        if ((n & (idx_t)1) == 0)           /* n even */
            return (pow - (seq_t)2) / (idx_t)3;
        else                               /* n odd  */
            return (pow + (seq_t)2) / (idx_t)3;
    }
}
