/*
 * A200439  Decimal expansion of constant arising in clubbed binomial
 *          approximation for the lightbulb process.
 * https://oeis.org/A200439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A200439: Decimal digits of 1352/495.
   a(1) = 2, a(2) = 7,
   a(n) = 3 if n is odd and n ≥ 3
   a(n) = 1 if n is even and n ≥ 4. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;
    /* For n ≥ 3, the decimal expansion repeats 3,1. */
    return (n & 1ULL) ? (seq_t)3 : (seq_t)1;
}
