/*
 * A163169  a(n) = minimal number of consecutive integers required which when
 *          summed make n.
 * https://oeis.org/A163169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = minimal k ≥ 2 such that ∃m ≥ 0 with n = k*(2m + k - 1)/2
 *            (sum of k consecutive integers starting at m). a(0) = 0. If no
 *            such k exists, return 0
 * Range:     u8 n <= 749, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 0 the definition is 0. */
    if (n == 0) return 0;

    seq_t two_n = (seq_t)n * 2;          /* 2n, stays within seq_t bounds */
    for (idx_t k = 2; ; ++k) {
        seq_t k_seq = (seq_t)k;
        /* Terminate if the minimal possible sum for length k exceeds n:
           minimal sum with m = 0 is k*(k-1)/2. */
        if (k_seq * (k_seq - 1) / 2 > (seq_t)n)
            break;

        /* Check divisibility and parity condition for m to be integer. */
        if (two_n % k_seq == 0) {
            seq_t temp = two_n / k_seq - (k_seq - 1);
            if (temp % 2 == 0) {
                seq_t m = temp / 2;
                if (m >= 0)
                    return k_seq;   /* minimal k found */
            }
        }
    }
    return 0;   /* no representation with at least two consecutive integers */
}
