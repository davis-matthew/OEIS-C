/*
 * A061727  A000217 interleaved with A061726.
 * https://oeis.org/A061727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, dumb, easy
 * Formula:   a(n) = T_{(n+1)/2} if n is odd; a(n) = T_{n/2} * 4 if (n/2) mod
 *            4 == 1 or 2, a(n) = T_{n/2} * 5 otherwise. Here T_k = k*(k+1)/2
 *            is the k-th triangular number
 * Range:     u8 n <= 21, u16 n <= 325, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* odd n: triangular number of m = (n+1)/2 */
    if (n & 1) {
        seq_t m = (seq_t)(n + 1) / 2;
        /* compute m*(m+1)/2 safely */
        seq_t t;
        if (m & 1) /* m odd */
            t = ((m + 1) / 2) * m;
        else /* m even */
            t = (m / 2) * (m + 1);
        return t;
    }

    /* even n: k = n/2 */
    seq_t k = (seq_t)(n / 2);
    /* triangular number of k */
    seq_t t;
    if (k & 1) /* k odd */
        t = ((k + 1) / 2) * k;
    else /* k even */
        t = (k / 2) * (k + 1);

    /* factor depends on k mod 4 */
    seq_t factor = ((k % 4 == 1 || k % 4 == 2) ? 4 : 5);
    return t * factor;
}
