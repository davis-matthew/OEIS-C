/*
 * A085741  a(n) = T(n)^n, where T() are the triangular numbers (A000217).
 * https://oeis.org/A085741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n*(n+1))/2)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0^0 is defined as 1 in the OEIS sequence */
    if (n == 0)
        return (seq_t)1;

    /* compute the triangular number T(n) = n*(n+1)/2 */
    seq_t t = ((seq_t)n) * (((seq_t)n) + 1) / 2;

    /* binary exponentiation of t^n */
    seq_t result = (seq_t)1;
    seq_t base   = t;
    idx_t exp    = n;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}
