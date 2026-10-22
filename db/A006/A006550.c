/*
 * A006550  n+8*C(n,2)+30*C(n,3)+62*C(n,4)+75*C(n,5)+30*C(n,6).
 * https://oeis.org/A006550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m + 8*C(m,2) + 30*C(m,3) + 62*C(m,4) + 75*C(m,5) +
 *            30*C(m,6), where m = n-1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   a006550_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006550_binom.h"

seq_t a(idx_t n) {
    idx_t m = n - 1;
    seq_t result = (seq_t)m;
    result += (seq_t)8 * a006550_binom(m, 2);
    result += (seq_t)30 * a006550_binom(m, 3);
    result += (seq_t)62 * a006550_binom(m, 4);
    result += (seq_t)75 * a006550_binom(m, 5);
    result += (seq_t)30 * a006550_binom(m, 6);
    return result;
}
