/*
 * A074723  Largest power of 2 dividing F(3n) where F(k) is the k-th Fibonacci
 *          number.
 * https://oeis.org/A074723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 , if n is odd * a(n) = 2^(v2(n)+2) , if n is even *
 *            where v2(n) is the exponent of the largest power of 2 dividing
 *            n. * This follows from the known 2‑adic valuation for Fibonacci
 *            numbers: * v2(F(3n)) = 1 if n is odd * v2(F(3n)) = v2(n) + 2 if
 *            n is even
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Odd indices give 2. */
    if (n & (idx_t)1)
        return (seq_t)2;

    /* Count factors of 2 in n (v2(n)). */
    idx_t m = 0;
    idx_t tmp = n;
    while ((tmp & (idx_t)1) == 0) {
        tmp >>= (idx_t)1;
        ++m;
    }

    /* Compute 2^(m+2) by repeated multiplication by 2.
     * This avoids undefined behaviour of shifting by large counts
     * and works for arbitrary‑precision seq_t. */
    seq_t result = (seq_t)2;      /* 2^(1) */
    /* Need to multiply by 2 (m + 1) more times to reach 2^(m+2). */
    idx_t times = m + 1;          /* since 1 + (m+1) = m+2 exponent */
    while (times--) {
        result *= (seq_t)2;
    }
    return result;
}
