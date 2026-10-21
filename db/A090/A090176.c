/*
 * A090176  Expansion of (1+x^9)/((1-x^4)*(1-x^6)*(1-x^12)).
 * https://oeis.org/A090176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of solutions to 4*i + 6*j + 12*k = n plus the same
 *            for n-9, where i, j, k are non‑negative integers
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   count_c
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_c.h"

seq_t a(idx_t n)
{
    seq_t ans = count_c(n);
    if (n >= 9) ans += count_c(n - 9);
    return ans;
}
