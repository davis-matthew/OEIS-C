/*
 * A029924  Convert n from kilograms (kg) to pounds (lbs).
 * https://oeis.org/A029924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/0.45359237 + 1/2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 21, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exact implementation using integer arithmetic: 1/0.45359237 = 100000000/45359237 */
/* Thus a(n) = floor(n * 100000000 / 45359237 + 1/2) */
/*          = (2 * n * 100000000 + 45359237) / (2 * 45359237) */

seq_t a(idx_t n)
{
    /* Rational constants for 1/0.45359237 */
    const seq_t M = 100000000;   /* numerator of reciprocal */
    const seq_t D = 45359237;    /* denominator of reciprocal */

    /* Compute rounded value: floor((2*n*M + D) / (2*D)) */
    seq_t num = (seq_t)2 * (seq_t)n * M + D;   /* numerator */
    seq_t den = (seq_t)2 * D;                   /* denominator */
    return num / den;
}
