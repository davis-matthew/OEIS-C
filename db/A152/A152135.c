/*
 * A152135  Maximal length of rook tour on an n X n+4 board.
 * https://oeis.org/A152135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 24*n^2 + 38*n + 9 + 3*(-1)^n)/6 (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic */
    seq_t ns = (seq_t)n;
    seq_t ns2 = ns * ns;          /* n^2 */
    seq_t ns3 = ns2 * ns;         /* n^3 */

    seq_t term1 = (seq_t)4 * ns3;  /* 4*n^3 */
    seq_t term2 = (seq_t)24 * ns2; /* 24*n^2 */
    seq_t term3 = (seq_t)38 * ns;  /* 38*n */
    seq_t term4 = (seq_t)9;        /* 9 */

    /* start with the sum of the positive terms */
    seq_t num = term1 + term2 + term3 + term4;

    /* add or subtract 3 depending on parity of n */
    if ((n & 1ULL) == 0ULL) {
        num += (seq_t)3;          /* n is even: +3 */
    } else {
        /* num is always at least 12 for n>=1, so unsigned subtraction is safe */
        num -= (seq_t)3;          /* n is odd: -3 */
    }

    /* divide by 6 (exact division for all n) */
    return num / (seq_t)6;
}
