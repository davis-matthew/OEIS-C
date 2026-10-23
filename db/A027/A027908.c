/*
 * A027908  a(n) = T(2*n, n), T given by A027907.
 * https://oeis.org/A027908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} C(2*n, n-k) * C(n-k, k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t two_n = n << 1;          /* 2*n (no overflow for n <= 10000) */
    seq_t total = 0;

    for (idx_t k = 0; k <= n / 2; ++k) {
        seq_t t1 = binomial(two_n, n - k);   /* C(2n, n-k) */
        seq_t t2 = binomial(n - k, k);       /* C(n-k, k) */
        total += t1 * t2;
    }
    return total;
}
