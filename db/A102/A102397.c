/*
 * A102397  a(n) = concatenation of first n elements of Thue-Morse sequence
 *          A010059.
 * https://oeis.org/A102397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum{k=0..n} ((1+(-1)^A010060(n-k)) / 2) * 10^k (where
 *            A010060(k) = popcount(k) mod 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   parity64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/parity64.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power   = 1;       /* 10^0 */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t m = n - k;                 /* n - k */
        /* If popcount(n-k) is even, we append '1' at position k. */
        if (parity64((uint64_t)m) == 0) {
            result += power;
        }
        power *= (seq_t)10;              /* advance to next decimal power */
    }
    return result;
}
