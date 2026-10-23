/*
 * A166337  a(n) = (2n+0^n)*C(4n,2n).
 * https://oeis.org/A166337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2n + 0^n) * C(4n,2n) (0^0 = 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (2*n-1)*(n-1)*a(n) - 2*(4*n-1)*(4*n-3)*a(n-1) = 0
   with a(0)=1, a(1)=12 */

#include <stdint.h>
#include "../utils/binom_n_k.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    seq_t bin = binom_n_k((idx_t)(4 * n), (idx_t)(2 * n));
    seq_t mult = (seq_t)2 * (seq_t)n;
    return bin * mult;
}
