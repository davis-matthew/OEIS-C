/*
 * A022533  Nexus numbers (n+1)^17 - n^17.
 * https://oeis.org/A022533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^17 - n^17
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 12, u128 n <= 213,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 9618 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    seq_t next_val = (seq_t)(n + 1);
    seq_t pow_next = ipow(next_val, (seq_t)17);
    seq_t pow_cur  = ipow(n_val, (seq_t)17);
    return pow_next - pow_cur;
}
