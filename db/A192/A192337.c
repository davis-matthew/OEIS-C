/*
 * A192337  Coefficient of x in the reduction of n-th polynomial at A157751 by
 *          x^2->x+1.
 * https://oeis.org/A192337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 6*a(n-3) - a(n-4) with initial
 *            values: a(1)=0, a(2)=1, a(3)=3, a(4)=10
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* guard for accidental 0, not used by OEIS offset */
    if (n == 1) return 0;          /* a(1) */
    if (n == 2) return 1;          /* a(2) */
    if (n == 3) return 3;          /* a(3) */
    if (n == 4) return 10;         /* a(4) */

    /* Sliding window of the 4 most recent terms. */
    seq_t a1 = 0;   /* a(n-4) */
    seq_t a2 = 1;   /* a(n-3) */
    seq_t a3 = 3;   /* a(n-2) */
    seq_t a4 = 10;  /* a(n-1) */

    seq_t an;
    for (idx_t k = 5; k <= n; ++k) {
        an = 4 * a4 - a3 - 6 * a2 - a1;
        a1 = a2;  /* shift window */
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
