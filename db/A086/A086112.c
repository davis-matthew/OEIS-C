/*
 * A086112  Denominator of the mean deviation of a discrete uniform
 *          distribution on n elements.
 * https://oeis.org/A086112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, changed
 * Formula:   a(n) = n if n is odd * a(n) = 1 if n ≡ 0 (mod 4) * a(n) = 2 if n
 *            ≡ 2 (mod 4)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A086112: Denominator of the mean deviation of a discrete uniform distribution on n elements. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Handle 0 as a safe fallback. */
    if (n == 0)
        return (seq_t)0;

    if ((n & 1) != 0)          /* odd n */
        return (seq_t)n;

    /* even n: let m = n/2 */
    idx_t m = n >> 1;          /* faster division by 2 */
    return (m & 1) ? (seq_t)2 : (seq_t)1;
}
