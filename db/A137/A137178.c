/*
 * A137178  a(n) = sum_(1..n) [S2(n)mod 2 - floor(5*S2(n)/7)mod 2], where
 *          S2(n) = binary weight of n.
 * https://oeis.org/A137178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_(k=1..n) (S2(k)mod 2 - floor(5*S2(k)/7)mod 2)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k)
    {
        unsigned int pc = __builtin_popcountll((unsigned long long)k);
        int s_mod2   = pc & 1;                /* S2(k) mod 2 */
        int t        = (5 * pc) / 7;          /* floor(5*S2(k)/7) */
        int t_mod2   = t & 1;                 /* that value mod 2 */
        int f        = s_mod2 - t_mod2;       /* contribution of k */
        sum += (seq_t)f;
    }
    return sum;
}
