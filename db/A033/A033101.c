/*
 * A033101  Number of 4's when n is written in base b for 2<=b<=n+1.
 * https://oeis.org/A033101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 4's in the base‑b representation of n for * all
 *            bases b with 2 ≤ b ≤ n+1
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   count_in_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_in_base.h"

/* Required function: computes a(n) as defined above. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t b = 2; b <= n + 1; ++b) {
        total += count_in_base(n, b);
    }
    return total;
}
