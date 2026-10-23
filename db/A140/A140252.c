/*
 * A140252  Inverse binomial transform of A140420.
 * https://oeis.org/A140252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*2^n - (-2)^n - 4)/4 for n >= 1, a(0) = 0 This
 *            simplifies to: a(0) = 0 a(n) = (n is even) ? (2^(n-1) - 1) :
 *            (2^n - 1) for n >= 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    if ((n & 1) == 0) {            // n is even
        seq_t p = pow2(n - 1);     // 2^(n-1)
        return p - 1;
    } else {                       // n is odd
        seq_t p = pow2(n);         // 2^n
        return p - 1;
    }
}
