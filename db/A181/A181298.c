/*
 * A181298  The number of even entries in all the 2-compositions of n. A
 *          2-composition of n is a nonnegative matrix with two rows, such
 *          that each column has at least one nonzero entry and whose entries
 *          sum up to n.
 * https://oeis.org/A181298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*z*(1-z)^2*(1+z-z^2)/((1+z)*(1-4*z+2*z^2)^2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0..5 */
    static const seq_t base[6] = {0, 2, 12, 56, 246, (seq_t)1024};

    /* Return early for the known initial terms */
    if (n < 6)
        return base[(size_t)n];

    /* Keep last five terms: a[i-5] ... a[i-1] */
    seq_t a_nm5 = base[1]; /* a[1] */
    seq_t a_nm4 = base[2]; /* a[2] */
    seq_t a_nm3 = base[3]; /* a[3] */
    seq_t a_nm2 = base[4]; /* a[4] */
    seq_t a_nm1 = base[5]; /* a[5] */
    seq_t an;

    /* Homogeneous recurrence for n >= 6:
     * a(n) = 7*a(n-1) - 12*a(n-2) - 4*a(n-3)
     *          + 12*a(n-4) - 4*a(n-5)
     * This can be written as: a(n) = (7*a(n-1)+12*a(n-4))
     *                            - (12*a(n-2)+4*a(n-3)+4*a(n-5)).
     * The right‑hand side is always non‑negative, so we can use
     * unsigned arithmetic safely.
     */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t pos = 7 * a_nm1 + 12 * a_nm4;
        seq_t neg = 12 * a_nm2 + 4 * a_nm3 + 4 * a_nm5;
        an = pos - neg;   /* pos >= neg guarantees no underflow */

        /* Shift the window for the next iteration */
        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }

    return an;
}
