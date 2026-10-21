/*
 * A202565  Indices of decagonal numbers which are also pentagonal.
 * https://oeis.org/A202565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 98*a(n-1)-a(n-2)-36, a(1)=1, a(2)=56
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* behaviour below offset is irrelevant */
    if (n == 1) return 1;
    if (n == 2) return 56;

    seq_t prev2 = 1;   /* a(1) */
    seq_t prev1 = 56;  /* a(2) */
    seq_t cur   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)98 * prev1 - prev2 - (seq_t)36;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
