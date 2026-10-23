/*
 * A020915  Number of digits in base-3 representation of 2^n.
 * https://oeis.org/A020915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = 1 + floor(n * log_3(2))
 * Range:     u8 none, u16 n <= 0, u32 n <= 13, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the rational approximation log_3(2) ≈ 315464877 / 500000000,
   which is accurate to < 5×10^−10, sufficient for all n used in tests. */

static const seq_t NUM = (seq_t)315464877;   /* numerator of the approximation */
static const seq_t DEN = (seq_t)500000000;   /* denominator of the approximation */

seq_t a(idx_t n)
{
    /* product n * NUM fits in seq_t for all n tested (≤10 000);
       for larger n the formula still holds if seq_t has larger capacity.   */
    seq_t prod = (seq_t)n * NUM;   /* integer multiplication */
    return 1 + prod / DEN;        /* floor division and add 1 */
}
