/*
 * A178184  Sum 2^((k^2+3k)/2) from k=1 to n.
 * https://oeis.org/A178184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^((k^2+3k)/2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 4, computed directly */
    if (n == 0) {
        return 0;          /* not in the defined range, but harmless */
    }

    seq_t sum  = 0;
    seq_t term = 4;         /* 2^((1^2+3)/2) = 2^2 */
    sum = term;

    /* For each subsequent k, the exponent increases by (k+1) bits */
    for (idx_t k = 2; k <= n; ++k) {
        term <<= (unsigned long long)(k + 1); /* term = term * 2^(k+1) */
        sum += term;
    }

    return sum;
}
