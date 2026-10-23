/*
 * A027656  Expansion of 1/(1-x^2)^2 (included only for completeness - the
 *          policy is always to omit the zeros from such sequences).
 * https://oeis.org/A027656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+2)*(1+(-1)^n))/4 Equivalent: a(n)=0 if n is odd;
 *            a(n)=n/2+1 if n is even
 * Range:     u8 n <= 509, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1ULL) /* odd index */
        return (seq_t)0;
    return ((seq_t)n) / 2 + (seq_t)1;
}
