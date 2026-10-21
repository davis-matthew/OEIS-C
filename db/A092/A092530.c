/*
 * A092530  a(0) = 0; for n > 0, a(n) = T(n) + k where T(n) is the n-th
 *          triangular number (A000217) and k (see A026741) is the smallest
 *          positive number such that a(n) is divisible by n.
 * https://oeis.org/A092530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; for n>0, a(n) = T(n) + k, where T(n) = n*(n+1)/2 and k
 *            is the smallest positive integer such that (T(n)+k) is divisible
 *            by n (k = n if T(n) is already divisible by n, otherwise k = n –
 *            T(n)%n)
 * Range:     u8 n <= 21, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;   /* n-th triangular number */
    seq_t r   = tri % (seq_t)n;                 /* tri mod n */
    seq_t k;
    if (r == 0)
        k = (seq_t)n;                           /* smallest positive k is n itself */
    else
        k = (seq_t)n - r;                       /* k = n – (tri % n) */
    return tri + k;
}
