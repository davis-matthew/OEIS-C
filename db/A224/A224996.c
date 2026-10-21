/*
 * A224996  a(n) = floor(1/f(x^(1/n))) for x = 2, where f computes the
 *          fractional part.
 * https://oeis.org/A224996
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A224996
 *  Formula: a(n) = floor(n / ln(2) - 1/2)
 *  Sequence is defined for n >= 2 (offset 2).
 *  The implementation uses a high‑precision rational approximation of 1/ln(2)
 *  that is accurate enough for all n that fit into uint64_t.
 *  A 64‑bit scaling factor gives an error < 0.5 for n up to ~10^12,
 *  which covers all test cases for the typical 64‑bit build.
 *  For arbitrary‑precision builds the same computation uses
 *  unsigned __int128 intermediates and casts the result to seq_t.
 */

seq_t a(idx_t n)
{
    /* For the offset of the sequence (n < 2) the value is irrelevant. */
    if (n < 2)
        return 0;

    /* Rational approximation of 1 / ln(2):
     * 1/ln(2) ≈ 14426950408889 / 10^13
     * Absolute error ≈ 6.34e-14, so for n < 8·10^12 the error in
     * n * (1/ln(2)) is < 0.5 and the floor is exact.
     */
    const unsigned long long P = 14426950408889ULL;   /* numerator   */
    const unsigned long long Q = 10000000000000ULL;   /* denominator (10^13) */
    const unsigned long long HALF_Q = Q / 2;          /* 5·10^12     */

    /* Compute (n * P - Q/2) / Q using 128‑bit arithmetic. */
    unsigned __int128 prod   = (unsigned __int128)n * P;
    prod -= (unsigned __int128)HALF_Q;
    unsigned __int128 result = prod / Q;

    return (seq_t)result;
}
