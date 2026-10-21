/*
 * A053405  Definition: A kara B = C, where C is the least nonnegative integer
 *          such that C * B >= A and C * (B-1) < A. a(n) is the smallest k
 *          such that n kara k is undefined.
 * https://oeis.org/A053405
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = smallest k ≥ 2 such that ceil(n/k)·(k‑1) ≥ n. For this k
 *            the “kara” operation n kara k is undefined, so a(n) is defined
 *            as that k
 * Range:     u8 n <= 263, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                    /* cast the index once */
    for (idx_t k = 2;; ++k)                /* k starts at 2 – k = 1 is always defined */
    {
        seq_t kk = (seq_t)k;
        seq_t C  = (nn + kk - 1) / kk;     /* ceil(n/k) */
        seq_t prod = C * (kk - 1);          /* C·(k‑1) */
        if (prod >= nn)                    /* condition for undefined */
            return kk;
    }
}
