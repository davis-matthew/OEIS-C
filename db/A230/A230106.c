/*
 * A230106  Number of m such that m + (product of nonzero digits of m) equals
 *          n.
 * https://oeis.org/A230106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of m (1 ≤ m ≤ n) such that * m + (product of
 *            nonzero digits of m) = n * (i.e. the number of representations
 *            of n as m plus the product of * the non‑zero digits of m)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* no positive m can give sum 0 */

    seq_t count = 0;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t prod = 1;              /* product of nonzero digits of m */
        idx_t t = m;
        while (t > 0) {
            unsigned digit = t % 10;     /* digit fits in unsigned */
            if (digit != 0) prod *= (seq_t)digit;
            t /= 10;
        }
        if ((seq_t)m + prod == (seq_t)n) ++count;
    }
    return count;
}
