/*
 * A099258  Inverse of A099257.
 * https://oeis.org/A099258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n = 3*2^k - 2 then n else (if n = 3*2^k - 3 then (n +
 *            1)/2 else n + 1)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t n_s = (seq_t)n;
    /* Case 1: n = 3*2^k - 2  ->  n+2 = 3*2^k  */
    seq_t m1 = n_s + 2;
    if (m1 % 3 == 0) {
        seq_t t = m1 / 3;
        if ((t & (t - 1)) == 0)  /* t is a power of 2 */
            return n_s;
    }
    /* Case 2: n = 3*2^k - 3  ->  n+3 = 3*2^k  */
    seq_t m2 = n_s + 3;
    if (m2 % 3 == 0) {
        seq_t t = m2 / 3;
        if ((t & (t - 1)) == 0)  /* t is a power of 2 */
            return (n_s + 1) / 2;
    }
    /* Default: n + 1 */
    return n_s + 1;
}
