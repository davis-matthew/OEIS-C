/*
 * A106617  Numerator of n/(n+16).
 * https://oeis.org/A106617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, mult, easy
 * Formula:   a(n) = n / gcd(n, 16)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for A106617.  The function is O(1) and works for any
 * integer type seq_t and idx_t defined by the build.  For n = 0 the
 * sequence is defined to be 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* a(0) = 0 */
    /* Determine how many trailing zero bits n has, up to a maximum
     * of 4, because gcd(n,16) = 2^k where k = min(v2(n), 4).        */
    uint8_t tz = 0;
    uint64_t tmp = n;
    while (tz < 4 && (tmp & 1) == 0) {
        ++tz;
        tmp >>= 1;
    }
    /* gcd = 1 << tz;  for tz == 0..4 this gives 1,2,4,8,16. */
    seq_t gcd = ((seq_t)1) << tz;
    return (seq_t)n / gcd;
}
