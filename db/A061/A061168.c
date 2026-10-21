/*
 * A061168  Partial sums of floor(log_2(k)) (= A000523(k)).
 * https://oeis.org/A061168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*floor(log2(n)) - 2*(2^floor(log2(n)) - 1)
 * Range:     u8 n <= 62, u16 n <= 6142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 for the defined range (offset 1) */
    /* compute floor(log2(n)) */
    idx_t k = 0;
    idx_t t = n;
    while (t >>= 1) {
        ++k;
    }

    /* 2^k as seq_t */
    seq_t pow2k = (seq_t)(1ULL << k);

    /* term1 = (n+1) * k */
    seq_t term1 = (seq_t)(n + 1) * (seq_t)k;

    /* term2 = 2 * (pow2k - 1) */
    seq_t term2 = (seq_t)2 * (pow2k - (seq_t)1);

    return term1 - term2;
}
