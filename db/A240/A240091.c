/*
 * A240091  a(n) = (10^n - 1)^n.
 * https://oeis.org/A240091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10^n - 1)^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   pow10, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;          /* a(0) = 1 by definition */
    seq_t base = pow10(n);         /* 10^n  */
    base -= 1;                     /* 10^n - 1 */
    return seq_pow(base, n);       /* (10^n - 1)^n */
}
