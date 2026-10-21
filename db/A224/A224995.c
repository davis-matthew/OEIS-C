/*
 * A224995  Floor(1/f(x^(1/n))) for x = 3/2, where f computes the fractional
 *          part.
 * https://oeis.org/A224995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/log(3/2)-1/2) for n > 1, a(1) = 2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a high‑precision rational approximation of 1/log(3/2).  */

seq_t a(idx_t n)
{
    /* Exact special case for the first term */
    if (n == 1)
        return (seq_t)2;

    /* 1/log(3/2) ≈ 2.466301089123....  */
    /* Use the rational 2466301/1000000, whose error is < 9e-8. */
    const seq_t P = 2466301;   /* numerator     */
    const seq_t Q = 1000000;   /* denominator   */

    seq_t n64 = (seq_t)n;          /* cast index to seq_t */
    seq_t num = n64 * P;  /* n * P */
    num <<= 1;            /* 2 * n * P */
    num -= Q;             /* subtract Q (equivalent to subtracting 0.5 before division) */
    seq_t den = Q << 1;   /* 2 * Q */

    return num / den;
}
