/*
 * A029300  Expansion of 1/((1-x^3)*(1-x^6)*(1-x^8)*(1-x^12)).
 * https://oeis.org/A029300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 6, 8, and 12 a(n)
 *            = Σ_{d=0}^{⌊n/12⌋} Σ_{c=0}^{⌊(n-12d)/8⌋} [ (n-12d-8c) mod 3 == 0
 *            ] * (⌊(n-12d-8c)/6⌋ + 1) where [P] is 1 if condition P is true,
 *            else 0. This follows from solving 3a+6b = rem, giving
 *            floor(rem/6)+1 solutions
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ans = 0;
    idx_t max_d = n / 12;
    for (idx_t d = 0; d <= max_d; ++d) {
        idx_t rest_d = n - 12 * d;
        idx_t max_c = rest_d / 8;
        for (idx_t c = 0; c <= max_c; ++c) {
            idx_t rem = rest_d - 8 * c;          // remainder after choosing c
            if (rem % 3 != 0) continue;          // not solvable by 3 and 6 parts
            idx_t t = rem / 3;                   // t = a + 2b
            idx_t sol = t / 2 + 1;               // number of nonnegative solutions (a,b)
            ans += (seq_t)sol;                   // accumulate
        }
    }
    return ans;
}
