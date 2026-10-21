/*
 * A094160  Column 4 of A048790.
 * https://oeis.org/A094160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (64*(n-1)^3 - 90*(n-1)^2 + 38*(n-1))/3 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 429, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Shift the index because the polynomial is expressed for n-1. */
    seq_t m  = (seq_t)n - 1;          /* m = n-1 */

    /* Compute m^2 and m^3 using only integer arithmetic. */
    seq_t m2 = m * m;                 /* m^2 */
    seq_t m3 = m2 * m;                /* m^3 */

    /* Numerator of the rational expression, integer for all n. */
    seq_t num = (seq_t)64 * m3
              - (seq_t)90 * m2
              + (seq_t)38 * m;

    /* Divide by 3 – exact division for all integer n. */
    return num / (seq_t)3;
}
