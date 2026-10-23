/*
 * A145119  a(n) = Product_{k=1..n-1} (ceiling(n/k) - ceiling(n/k) mod 2).
 * https://oeis.org/A145119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product_{k=1}^{n-1} (ceil(n/k) - (ceil(n/k) mod 2))
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 1;            /* a(1) = 1 by definition */
    seq_t result = 1;
    for (idx_t k = 1; k < n; ++k) {
        seq_t m = ((seq_t)n + (seq_t)k - 1) / (seq_t)k;  /* ceil(n/k) */
        if (m & 1) m -= 1;                          /* make term even */
        result *= m;
    }
    return result;
}
