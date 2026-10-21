/*
 * A221920  a(n) = 3*n/gcd(3*n, n+3), n >= 3.
 * https://oeis.org/A221920
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n/gcd(3*n,n+3)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 3) {
        return 0; /* Undefined below the offset; return 0 */
    }
    seq_t num = (seq_t)3 * (seq_t)n;
    seq_t den = gcd((seq_t)9, (seq_t)(n + 3));
    return num / den;
}
