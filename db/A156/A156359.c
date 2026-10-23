/*
 * A156359  Denominator of Euler(n, 1/14).
 * https://oeis.org/A156359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0) = 1, a(n) = 7 * a(n-1) if n is odd, a(n) = 28 * a(n-1) if n
 *            is even Equivalently, a(n) = 7^ceil(n/2) * 28^floor(n/2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        res = res * ((i & 1ULL) ? (seq_t)7 : (seq_t)28);
    }
    return res;
}
