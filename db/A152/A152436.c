/*
 * A152436  a(n) = (13^n - 1)/(3*2^(3 - (n mod 2))).
 * https://oeis.org/A152436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 170*a(n-2) - 169*a(n-4) a(0)=0, a(1)=1, a(2)=7, a(3)=183
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle first four terms directly */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 7;
    if (n == 3) return 183;

    /* Keep the last four computed values.
       last4[0]=a(n-4), last4[1]=a(n-3),
       last4[2]=a(n-2), last4[3]=a(n-1)   */
    seq_t last4[4] = {0, 1, 7, 183};

    idx_t k;
    for (k = 4; k <= n; ++k) {
        /* a(k) = 170*a(k-2) - 169*a(k-4) */
        seq_t ak = 170 * last4[2] - 169 * last4[0];

        /* Shift window for next iteration */
        last4[0] = last4[1];
        last4[1] = last4[2];
        last4[2] = last4[3];
        last4[3] = ak;
    }

    return last4[3];
}
