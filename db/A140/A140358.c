/*
 * A140358  Smallest nonnegative integer k such that n = +-1+-2+-...+-k for
 *          some choice of +'s and -'s.
 * https://oeis.org/A140358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = min{k ≥ 0 : T(k) ≥ n and (T(k) mod 2 = n mod 2)}, where
 *            T(k) = k(k+1)/2. If the parity does not match, the answer is the
 *            smallest odd integer > k
 * Range:     u8 n <= 9322, u16 n <= 9322, u32 n <= 9322, u64 n <= 9322,
 *            u128 n <= 9322, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* compute a(n) according to the definition of OEIS A140358 */
seq_t a(idx_t n)
{
    /* Handle the trivial case directly. */
    if (n == 0) return (seq_t)0;

    /* Use the closed‑form estimate for the minimal k with T(k) ≥ n. */
    long double d = sqrtl((long double)8 * n + 1.0L);
    idx_t k = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct any rounding error by moving k upwards until the
       triangular number is at least n.  The comparison uses 128‑bit
       arithmetic to avoid overflow. */
    while (( (__int128)k * (k + 1) / 2 ) < n) k++;

    /* Parity of the triangular number can be derived from k mod 4:
       T(k) is odd when k%4 is 1 or 2, even otherwise. */
    idx_t parityTk = (k % 4 == 1 || k % 4 == 2) ? 1 : 0;

    /* Parity of n. */
    idx_t parityN = n & 1U;

    /* If parities match, k is the answer. */
    if (parityTk == parityN) return (seq_t)k;

    /* Otherwise return the least odd integer greater than k. */
    return (k & 1U) ? (seq_t)(k + 2) : (seq_t)(k + 1);
}
