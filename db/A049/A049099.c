/*
 * A049099  a(n) = Euler phi function applied thrice to n.
 * https://oeis.org/A049099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000010(A000010(A000010(n))) // Euler totient applied
 *            three times
 * Range:     u8 n <= 1095, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/calc_phi.h"

seq_t a(idx_t n) {
    /* Convert the index to seq_t once, then apply phi three times */
    seq_t x = (seq_t)n;          /* first argument to calc_phi */
    x = calc_phi(x);             /* φ(n)   */
    x = calc_phi(x);             /* φ(φ(n)) */
    x = calc_phi(x);             /* φ(φ(φ(n))) */
    return x;
}
