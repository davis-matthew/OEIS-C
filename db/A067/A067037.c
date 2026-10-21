/*
 * A067037  a(n) = n^m where m = floor(Sum_{k=1..n} 1/k).
 * https://oeis.org/A067037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n ^ floor(H_n)
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 84, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where H_n = 1 + 1/2 + ... + 1/n is the n-th harmonic number. */
seq_t a(idx_t n)
{
    /* Compute m = floor(H_n) using a running double sum. */
    double sum = 0.0;
    idx_t i;
    for (i = 1; i <= n; ++i)
        sum += 1.0 / (double)i;

    idx_t m = 0;
    while ((double)(m + 1) <= sum)
        ++m;

    /* Compute n^m by repeated multiplication.  The exponent m is
       small (≤10 for n ≤ 10 000), so a simple loop is efficient and
       keeps the arithmetic within the seq_t type. */
    seq_t result = 1;
    seq_t base   = (seq_t)n;
    for (i = 0; i < m; ++i)
        result *= base;

    return result;
}
