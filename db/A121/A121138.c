/*
 * A121138  Numbers of isomers of unbranched a-4-catapolyheptagons - see
 *          Brunvoll reference for precise definition.
 * https://oeis.org/A121138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(1)=1 , a(2)=1 , a(3)=3 ; * for n>3: * if n is even:
 *            a(n)=4^(n-4)*(n+6); * if n is odd: a(n)=2^(n-8)*(2^n*(n+6)+16)
 *            (exact division for n<8)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* offset terms */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* n > 3 */
    if ((n & 1) == 0) {                    /* even n */
        return pow4(n - 4) * (seq_t)(n + 6);
    } else {                               /* odd n */
        seq_t term = pow2(n) * (seq_t)(n + 6) + (seq_t)16;
        long long diff = (long long)n - 8;  /* n-8; may be negative */
        if (diff >= 0) {
            return pow2((idx_t)diff) * term;
        } else {
            return term / pow2((idx_t)(-diff));
        }
    }
}
