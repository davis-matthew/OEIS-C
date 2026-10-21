/*
 * A078191  a(n) = concatenation of n n times divided by n.
 * https://oeis.org/A078191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (10^(d*n) - 1)/(10^d - 1) where d = number of decimal
 *            digits of n, which equals the integer formed by concatenating
 *            the decimal representation of n exactly n times, divided by n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS sequence A078191. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but guard against n==0 */
        return 0;

    /* Count digits of n (base 10). */
    idx_t tmp = n;
    idx_t d = 0;
    while (tmp > 0) {
        ++d;
        tmp /= 10;
    }

    /* Compute 10^d as a seq_t. */
    seq_t base = 1;
    for (idx_t i = 0; i < d; ++i)
        base *= 10;

    /* Compute the repunit S = 1 + base + base^2 + ... + base^(n-1). */
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i)
        result = result * base + 1;

    return result;
}
