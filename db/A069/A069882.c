/*
 * A069882  Numbers n such that n and 2n-1 are both palindromes.
 * https://oeis.org/A069882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = n for 1 ≤ n ≤ 6 a(n) = 2 * (10^(n-5) – 1) / 3 for n > 5 *
 *            Exact computation for any seq_t (int, long long, __int128, or
 *            arbitrary‑precision)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 6) {
        return (seq_t)n;
    }
    idx_t k = n - 5;               /* number of digits of the result (all sixes) */
    seq_t p = pow10(k);            /* 10^k */
    seq_t tmp = (p - 1) / 3;       /* (10^k – 1) / 3  is an integer */
    return tmp * 2;                /* multiply by 2 to get the final value */
}
