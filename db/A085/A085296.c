/*
 * A085296  Runs of zeros in Catalan sequence modulo 3: consecutive
 *          occurrences of binomial(2*k,k)/(k+1) == 0 (mod 3).
 * https://oeis.org/A085296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n-1) = 3, a(2*n) = 3 * (a(n)+1) (for n ≥ 1)
 * Range:     u8 n <= 15, u16 n <= 511, u32 n <= 9499, u64 n <= 9499,
 *            u128 n <= 9499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Thus a(n) = 3 if n is odd; otherwise a(n) = 3*(a(n/2)+1)   */
seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    if (n & 1)           /* odd index */
        return 3;
    else
        return 3 * (a(n >> 1) + 1);
}
