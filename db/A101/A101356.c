/*
 * A101356  Binomial( 2*binomial(2*n,n-1),n-1).
 * https://oeis.org/A101356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Binomial( 2*binomial( 2*n+2 , n ) , n )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   binom, binom_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/binom_big.h"

seq_t a(idx_t n) {
    /* inner binomial C(2*n+2, n) */
    idx_t N = 2*n + 2;
    seq_t inner = binom(N, n);
    /* top of outer binomial: 2 * inner */
    seq_t top = inner * 2;
    /* outer binomial C(top, n) */
    seq_t result = binom_big(top, (seq_t)n);
    return result;
}
