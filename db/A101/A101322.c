/*
 * A101322  a(n) = n - (least divisor of n >= the square root of n) +
 *          (greatest divisor of n <= the square root of n).
 * https://oeis.org/A101322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (least divisor of n ≥ sqrt(n)) + (greatest divisor of
 *            n ≤ sqrt(n))
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt.h"

seq_t a(idx_t n) {
    /* n ≥ 1 in normal use; for n = 0 return 0 as a guard */
    if (n == 0) return 0;

    /* floor of the integer square root of n */
    idx_t r = int_sqrt(n);

    /* greatest divisor ≤ sqrt(n) – search downward from r */
    idx_t g = 1;
    for (idx_t i = r; i > 0; --i) {
        if (n % i == 0) {
            g = i;
            break;
        }
    }

    /* convert to seq_t and compute a(n) */
    seq_t n_seq = (seq_t)n;
    seq_t g_seq = (seq_t)g;
    seq_t l_seq = n_seq / g_seq;       /* least divisor ≥ sqrt(n) */
    return n_seq - l_seq + g_seq;
}
