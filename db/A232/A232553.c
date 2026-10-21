/*
 * A232553  Maximal values of permanent on (0,1) square matrices of order n
 *          with row and column sums 3.
 * https://oeis.org/A232553
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3)=6, a(4)=9, a(5)=13, a(6)=36, a(7)=54, a(8)=81; * a(n) = 6 *
 *            a(n-3) for all n > 8
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 74, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage function required by the task */
seq_t a(idx_t n)
{
    /* Return 0 for indices before the offset – behaviour undefined is acceptable. */
    if (n < 3)
        return 0;

    /* Base values for n = 3, 4, 5, 6, 7, 8 */
    static const seq_t base[6] = {6, 9, 13, 36, 54, 81};

    /* Small indices – return directly. */
    if (n <= 8)
        return base[n - 3];

    /* Reduce by 3 until we reach the base range, counting the multiplicative factor. */
    idx_t m = n;
    seq_t fact = 1;
    while (m > 8) {
        fact *= 6;   /* multiply by 6 for each 3-step jump upward */
        m    -= 3;
    }

    return fact * base[m - 3];
}
