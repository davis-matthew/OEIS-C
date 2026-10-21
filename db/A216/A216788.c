/*
 * A216788  a(n) = Product_{k=1..n} (169 - 13/k).
 * https://oeis.org/A216788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (169 - 13/k) (with a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) * (169*n-13) / n  (division exact) */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t factor = (seq_t)169 * k - 13;  /* 169*k - 13  */
        result = result * factor;
        result = result / k;                /* exact division */
    }
    return result;
}
