/*
 * A225790  Rubik's Square sequence.
 * https://oeis.org/A225790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n≥2 let k=floor(n/2);
 *            a(2k)=12^(k^2)*2^(2k-1)=2^(2k^2+2k-1)*3^(k^2);
 *            a(2k+1)=4*a(2k)=2^(2k^2+2k+1)*3^(k^2)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    idx_t k = n / 2;
    idx_t exp3 = k * k;
    idx_t exp2;
    if ((n & 1) == 0) {           // even n = 2k
        exp2 = 2 * k * k + 2 * k - 1;
    } else {                      // odd n = 2k+1
        exp2 = 2 * k * k + 2 * k + 1;
    }
    seq_t v2 = pow2(exp2);
    seq_t v3 = pow3(exp3);
    return v2 * v3;
}
