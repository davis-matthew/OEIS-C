/*
 * A164632  a(1) = 1 followed by 2^k appearing 2^(2*k-1) times for k>0.
 * https://oeis.org/A164632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^k where k is the smallest integer such that (2·4^k +
 *            1)/3 ≥ n. This is equivalent to having the cumulative count S(k)
 *            = 1 + Σ_{i=1}^{k} 2·4^{i-1} satisfy S(k) ≥ n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n >= 1 in practice */

    /* Use idx_t for counting to avoid overflow in seq_t (particularly
       for small seq_t types like uint8_t). */
    idx_t S    = 1;            /* cumulative length up to current k */
    idx_t pow4 = 1;            /* 4^k  (k starts at 0) */
    idx_t k    = 0;

    if (S >= n) return pow2((idx_t)k);

    for (k = 1; ; ++k) {
        /* m_k = 2·4^(k-1) = 2·pow4 */
        S += (idx_t)2 * pow4;
        if (S >= n) return pow2(k);
        pow4 *= 4;            /* advance pow4 to 4^k for next iteration */
    }
}
