/*
 * A091512  a(n) is the largest integer m such that 2^m divides (2*n)^n, i.e.,
 *          the exponent of 2 in (2*n)^n.
 * https://oeis.org/A091512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n * ( 1 + v2(n) ) * where v2(n) is the exponent of 2 in
 *            the prime factorisation of n * (i.e. the number of trailing zero
 *            bits in the binary representation of n)
 * Range:     u8 n <= 63, u16 n <= 6143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n==0 – behaviour for n<1 is undefined by the spec,
       but we guard against it since a(0) is unused. */
    if (n == 0)
        return 0;

    /* Compute v2(n) – the number of factors of 2 dividing n.
       For a 64‑bit integer at most 64 iterations are needed. */
    idx_t temp = n;
    idx_t tz = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        tz++;
    }

    /* a(n) = n * (tz + 1).  All arithmetic is performed in seq_t. */
    return (seq_t)n * ((seq_t)tz + 1);
}
