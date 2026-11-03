/*
 * A024119  a(n) = 10^n - n^5.
 * https://oeis.org/A024119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - n^5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 272 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    seq_t ten_pow = ipow((seq_t)10, (seq_t)n);   /* 10^n */
    seq_t n_seq = (seq_t)n;
    seq_t n2   = n_seq * n_seq;   /* n^2 */
    seq_t n3   = n2   * n_seq;    /* n^3 */
    seq_t n4   = n3   * n_seq;    /* n^4 */
    seq_t n5   = n4   * n_seq;    /* n^5 */
    return ten_pow - n5;          /* 10^n - n^5 */
}
