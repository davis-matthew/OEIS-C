/*
 * A004951  a(n) = round(n*phi^16), where phi is the golden ratio, A001622.
 * https://oeis.org/A004951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^16) = 2207*n - round(n*phi^(-16)), since
 *            phi^16 = 2207 - phi^(-16) = (2207 + 987*sqrt(5))/2;
 *            round(n*phi^(-16)) = floor((2207*n + 1 - 987*n*sqrt(5))/2),
 *            computed exactly with integer sqrt
 * Range:     u8 n <= 0, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   a004951_isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004951_isqrt128.h"

seq_t a(idx_t n)
{
    unsigned __int128 A, B, r, m, k;
    if (n == 0)
        return (seq_t)0;
    A = (unsigned __int128)n * 2207 + 1;
    B = (unsigned __int128)n * 987;
    /* m = ceil(B*sqrt(5)); 5*B^2 is never a perfect square for B > 0 */
    r = a004951_isqrt128(5 * B * B);
    m = r + 1;
    k = (A - m) / 2;
    return (seq_t)n * 2207 - (seq_t)(uint64_t)k;
}
