/*
 * A016172  Expansion of 1/((1-6*x)*(1-9*x)).
 * https://oeis.org/A016172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 6^n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is the only base case */
    if (n == 0) return (seq_t)1;

    /* current value of a(k) (initially a(0)) */
    seq_t cur = (seq_t)1;

    /* current power 6^k (initially 6^0) */
    seq_t pow6 = (seq_t)1;

    /* iterate from k = 1 up to n */
    for (idx_t k = 1; k <= n; ++k) {
        /* update pow6 to 6^k */
        pow6 = pow6 * (seq_t)6;
        /* apply recurrence: a(k) = 9*a(k-1) + 6^k */
        cur = (seq_t)9 * cur + pow6;
    }

    return cur;
}
