/*
 * A053715  a(n) = n-th triangular number (the sum of the first n integers) in
 *          base n.
 * https://oeis.org/A053715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   for n = 0 → 0, n = 1 → 1 for n > 1: if n is even: a(n) = 11 * n
 *            / 2 if n is odd: a(n) = 5 * (n + 1) (i.e. a(n) = 5n + 5)
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This matches the OEIS formula: a(n) = (-10*(-1+(-1)^n)+(21+(-1)^n)*n)/4 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t N = (seq_t)n;

    if ((N & 1) == 0)          /* even n */
        return (N * 11) / 2;   /* 11*n/2 */

    return (N + 1) * 5;       /* 5*(n+1) = 5n+5 */
}
