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
 * Validated: standard, pass, 117 terms through n = 9962 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/totient_seq.h"

seq_t a(idx_t n) {
    /* Convert the index to seq_t once, then apply phi three times */
    seq_t x = (seq_t)n;          /* first argument to calc_phi */
    x = totient_seq(x);             /* φ(n)   */
    x = totient_seq(x);             /* φ(φ(n)) */
    x = totient_seq(x);             /* φ(φ(φ(n))) */
    return x;
}
