/*
 * A193658  Q-residue of the triangle A051162, where Q is the triangular array
 *          (t(i,j)) given by t(i,j)=1. (See Comments.)
 * https://oeis.org/A193658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+4)*a(n-1) - (4*n-1)*a(n-2) + 5*(n-2)*a(n-3) -
 *            2*(n-3)*a(n-4) Initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=15.
 *            This implements the linear recurrence found in the OEIS entry
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)15;

    /* Keep only the four most recent values. */
    seq_t a_nm4 = (seq_t)1;   /* a(n-4) */
    seq_t a_nm3 = (seq_t)1;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)3;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)15;  /* a(n-1) */
    seq_t a_curr;

    for (idx_t k = 4; k <= n; ++k) {
        seq_t pos = (seq_t)(k + 4) * a_nm1 + (seq_t)(5 * (k - 2)) * a_nm3;
        seq_t neg = (seq_t)(4 * k - 1) * a_nm2 + (seq_t)(2 * (k - 3)) * a_nm4;
        a_curr = pos - neg;

        /* Slide the window for the next iteration. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_curr;
    }

    return a_nm1;
}
