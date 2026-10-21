/*
 * A052793  Expansion of e.g.f. x^4*exp(x)^2 - 2*x^4*exp(x) + x^4.
 * https://oeis.org/A052793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0, for n < 6 a(n) = n*(n-1)*(n-2)*(n-3) * (2^(n-4) - 2),
 *            for n >= 6
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 19, u64 n <= 46, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the EGF:  x^4*(exp(2x) - 2*exp(x) + 1).  */
seq_t a(idx_t n)
{
    /* Terms for n < 6 are zero. */
    if (n < 6)
        return 0;

    /* Compute n*(n-1)*(n-2)*(n-3) as a seq_t. */
    seq_t prod = (seq_t)n *
                (seq_t)(n - 1) *
                (seq_t)(n - 2) *
                (seq_t)(n - 3);

    /* Compute 2^(n-4) - 2.  We use repeated doubling to stay within the
       limits of seq_t.  This is O(n) and is fine for n up to ~10^4. */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n - 4; ++i)
        pow2 *= (seq_t)2;
    pow2 -= (seq_t)2;

    return prod * pow2;
}
