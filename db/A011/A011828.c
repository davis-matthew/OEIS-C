/*
 * A011828  Number of f-vectors for simplicial complexes of dimension at most
 *          3 on at most n-1 vertices.
 * https://oeis.org/A011828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (12*(n-1)^10 -112*(n-1)^9 +351*(n-1)^8 -132*(n-1)^7 *
 *            +378*(n-1)^6 -2856*(n-1)^5 +4839*(n-1)^4 * +56812*(n-1)^3
 *            -5580*(n-1)^2 +309168*(n-1) +725760)/362880 * for n ≥ 1 (offset
 *            1)
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety, handle indices below the offset. */
    if (n == 0)
        return 0;

    /* Work with n' = n-1, as the polynomial is in terms of (n-1). */
    idx_t m = n - 1;
    seq_t x = (seq_t)m;            /* (n-1) as seq_t */

    /* Compute powers of x up to x^10.  For m = 0 all higher powers are 0. */
    seq_t p1  = x;            /* x^1          */
    seq_t p2  = p1 * x;       /* x^2          */
    seq_t p3  = p2 * x;       /* x^3          */
    seq_t p4  = p3 * x;       /* x^4          */
    seq_t p5  = p4 * x;       /* x^5          */
    seq_t p6  = p5 * x;       /* x^6          */
    seq_t p7  = p6 * x;       /* x^7          */
    seq_t p8  = p7 * x;       /* x^8          */
    seq_t p9  = p8 * x;       /* x^9          */
    seq_t p10 = p9 * x;       /* x^10         */

    /* Sum all positive contributions. */
    seq_t pos = 0;
    pos += (seq_t)12   * p10;
    pos += (seq_t)351  * p8;
    pos += (seq_t)378  * p6;
    pos += (seq_t)4839 * p4;
    pos += (seq_t)56812* p3;
    pos += (seq_t)309168* p1;
    pos += (seq_t)725760;

    /* Sum all negative contributions (to be subtracted). */
    seq_t neg = 0;
    neg += (seq_t)112  * p9;
    neg += (seq_t)132  * p7;
    neg += (seq_t)2856 * p5;
    neg += (seq_t)5580 * p2;

    /* Final polynomial value is pos - neg.  This is always non‑negative. */
    seq_t val = pos - neg;

    /* Divide by the constant 362880 to get the sequence term. */
    return val / 362880;
}
