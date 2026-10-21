/*
 * A215098  a(0)=0, a(1)=1, a(n) = n*(n-1) - a(n-2).
 * https://oeis.org/A215098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1) - a(n-2) (n >= 2) with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* a_prev_prev will hold a(k-2) and a_prev will hold a(k-1) */
    seq_t a_prev_prev = (seq_t)0;   /* a(0) */
    seq_t a_prev      = (seq_t)1;   /* a(1) */
    seq_t a_cur;

    /* Compute the values iteratively. */
    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = i*(i-1) - a(i-2) */
        a_cur = (seq_t)i * ((seq_t)i - 1) - a_prev_prev;
        a_prev_prev = a_prev;
        a_prev = a_cur;
    }
    return a_cur;
}
