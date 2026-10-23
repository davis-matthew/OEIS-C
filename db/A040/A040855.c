/*
 * A040855  Continued fraction for sqrt(885).
 * https://oeis.org/A040855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = 29 if n=0; otherwise a(n) = 2 if n is even with exactly
 *            one factor 2, a(n) = 58 if n has a factor 2^e with e>=2, else
 *            a(n) = 1 (n odd). Multiplicative with the given prime powers
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0u)
        return (seq_t)29;

    /* If n is odd, a(n)=1. */
    if ((n & (idx_t)1u) == (idx_t)1u)
        return (seq_t)1;

    /* Count the exponent of 2 in n. */
    idx_t e = (idx_t)0u;
    idx_t tmp = n;
    while ((tmp & (idx_t)1u) == (idx_t)0u) {
        ++e;
        tmp >>= 1;
    }

    if (e == (idx_t)1u)
        return (seq_t)2;
    else
        return (seq_t)58;
}
