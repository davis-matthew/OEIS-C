/*
 * A022148  Coordination sequence for root lattice B_6.
 * https://oeis.org/A022148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n>0, a(n) = (8*n/15)*(58*n^4 - 65*n^3 + 180*n^2 -
 *            85*n + 47)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Convert index to seq_t for arithmetic */
    seq_t nval = (seq_t)n;

    /* Compute powers of n */
    seq_t n2 = nval * nval;    /* n^2 */
    seq_t n3 = n2 * nval;      /* n^3 */
    seq_t n4 = n3 * nval;      /* n^4 */

    /* Positive part: 58*n^4 + 180*n^2 + 47 */
    seq_t part_pos = (seq_t)58 * n4 + (seq_t)180 * n2 + (seq_t)47;

    /* Negative part: 65*n^3 + 85*n */
    seq_t part_neg = (seq_t)65 * n3 + (seq_t)85 * nval;

    /* P(n) = part_pos - part_neg  (positive for all n ≥ 1) */
    seq_t p = part_pos - part_neg;

    /* Multiplier 8*n */
    seq_t mult = nval * (seq_t)8;

    /* Reduce the product 8*n*p by removing the prime factors of 15 = 3×5.
       At least one of mult or p is divisible by each prime. */
    if ((mult % (seq_t)3) == 0)
        mult /= (seq_t)3;
    else
        p /= (seq_t)3;

    if ((mult % (seq_t)5) == 0)
        mult /= (seq_t)5;
    else
        p /= (seq_t)5;

    /* Final result */
    return mult * p;
}
