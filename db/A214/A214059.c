/*
 * A214059  Least m>0 such that gcd(n^2+1+m, n-m) > 1.
 * https://oeis.org/A214059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m>0 such that gcd(n^2+1+m, n-m) > 1 (computed by
 *            linear search)
 * Range:     u8 n <= 15, u16 n <= 347, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Linear search for the smallest m>0 with the required property */
    for (idx_t m = 1; ; ++m) {
        /* Compute A = n^2 + 1 + m  and |B| = |n - m| */
        seq_t A = (seq_t)n * (seq_t)n + 1 + m;
        seq_t B_abs = (seq_t)(n >= m ? n - m : m - n);

        /* If B_abs is 0 (i.e., m = n), gcd(A,0) = A > 1, so we can return immediately */
        if (B_abs == (seq_t)0)
            return (seq_t)m;

        /* Check the gcd condition */
        if (gcd(A, B_abs) > (seq_t)1)
            return (seq_t)m;
    }
}
