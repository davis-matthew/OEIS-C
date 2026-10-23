/*
 * A127237  Row sums of Thue-Morse binomial triangle A127236.
 * https://oeis.org/A127237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} A010060(binomial(n,k))
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 30, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* helper for binomial computation (not used below) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t c   = 1;                  /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        /* parity of popcount of c */
        seq_t parity = 0;
        seq_t t = c;
        while (t != 0) {
            parity ^= (t & 1);
            t >>= 1;
        }
        sum += parity;              /* add 1 if popcount is odd, else 0 */

        if (k == n) break;          /* last element, no next binomial needed */

        /* compute C(n,k+1) from C(n,k) */
        c = (c * (seq_t)(n - k)) / (seq_t)(k + 1);
    }
    return sum;
}
