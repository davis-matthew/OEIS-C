/*
 * A124780  a(n) = gcd(A(n), A(n+2)) where A(n) = A000522(n) = Sum_{k=0..n}
 *          n!/k!.
 * https://oeis.org/A124780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A(n), A(n+2)) * where A(n) = sum_{k=0..n} n!/k! * *
 *            Using the recurrence A(n+2) = (n+2)(n+1) * A(n) + (n+3), * we
 *            only need A(n) modulo (n+3) to compute the gcd. * * Complexity:
 *            O(n) per call, n up to ~10000 works well
 * Range:     u8 n <= 16, u16 n <= 721, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* m = n + 3 */
    seq_t M = (seq_t)(n + 3);

    /* Handle small n explicitly */
    if (n == 0) {
        /* A(0) = 1 */
        return gcd((seq_t)1, M);
    }
    if (n == 1) {
        /* A(1) = 2 */
        return gcd((seq_t)2, M);
    }

    /* Recurrence:
     * A(0) = 1
     * A(1) = 2
     * For i >= 2:
     *   A(i) = i*(i-1)*A(i-2) + (i+1)
     */
    seq_t a_im2 = ((seq_t)1) % M;   /* A(0) mod M */
    seq_t a_im1 = ((seq_t)2) % M;   /* A(1) mod M */
    seq_t a_i   = a_im1;            /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute term = i*(i-1) mod M */
        seq_t term = ((seq_t)i) * ((seq_t)(i - 1));
        term %= M;

        /* A(i) mod M */
        a_i = (term * a_im2 + (seq_t)(i + 1)) % M;

        /* Shift for next iteration */
        a_im2 = a_im1;
        a_im1 = a_i;
    }

    return gcd(a_i, M);  /* a(n) = gcd(A(n), n+3) */
}
