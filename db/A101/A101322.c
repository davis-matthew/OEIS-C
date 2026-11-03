/*
 * A101322  a(n) = n - (least divisor of n >= the square root of n) +
 *          (greatest divisor of n <= the square root of n).
 * https://oeis.org/A101322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (least divisor of n ≥ sqrt(n)) + (greatest divisor of
 *            n ≤ sqrt(n))
 * Range:     u8 n <= 255, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 107 terms through n = 9966 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n) {
    /* n ≥ 1 in normal use; for n = 0 return 0 as a guard */
    if (n == 0) return 0;

    /* floor of the integer square root of n */
    idx_t r = isqrt(n);

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
