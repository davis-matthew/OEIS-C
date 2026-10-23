/*
 * A024922  a(n) = Sum_{k=1..n} floor((n/k) * floor((n/k) * floor(n/k))).
 * https://oeis.org/A024922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor((n/k) * floor((n/k) * floor(n/k)))
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* n/k and n%k are computed in the native idx_t type */
        seq_t q = (seq_t)(n / k);          /* floor(n/k) */
        seq_t r = (seq_t)(n % k);          /* n mod k       */
        seq_t A = q;                       /* A = floor(n/k) */
        seq_t B = r;                       /* B = n mod k   */
        /* C = floor(A * B / k) */
        seq_t C = (A * B) / (seq_t)k;
        /* v = A*A + C = floor(A * (A + B/k)) */
        seq_t v = A * A + C;
        /* term = A*v + floor(v * B / k) */
        seq_t term = A * v + (v * B) / (seq_t)k;
        sum += term;
    }
    return sum;
}
