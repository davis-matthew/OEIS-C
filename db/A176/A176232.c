/*
 * A176232  Determinant of the n X n matrix with rows (1!,-1,0,...,0), (1,
 *          2!,-1,0,...,0), (0,1,3!,-1,0,...,0), ..., (0,0,...,1,n!).
 * https://oeis.org/A176232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * a(n-1) + a(n-2), a(0)=1, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;

    seq_t a0 = 1;   /* a(n-2) */
    seq_t a1 = 1;   /* a(n-1) */
    seq_t fact = 1; /* (k-1)! */

    for (idx_t k = 2; k <= n; ++k) {
        fact *= (seq_t)k;          /* k! */
        seq_t an = fact * a1 + a0; /* a(k) */
        a0 = a1;
        a1 = an;
    }

    return a1;
}
