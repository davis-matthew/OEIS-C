/*
 * A088502  Numbers n such that (n^2 - 5)/4 is prime.
 * https://oeis.org/A088502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = odd integer n such that (n^2 - 5)/4 is prime
 * Range:     u8 n <= 6, u16 n <= 56, u32 n <= 133, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    seq_t cand = (seq_t)5;               /* first solution */
    idx_t count = 1;
    if (n == 1) return cand;
    while (1) {
        cand += (seq_t)2;                /* next odd candidate */
        seq_t sq = cand * cand;
        seq_t num = sq - (seq_t)5;
        seq_t p   = num / (seq_t)4;      /* (cand^2 - 5)/4, integer for odd cand */
        if (is_prime_big(p)) {
            count++;
            if (count == n) return cand;
        }
    }
}
