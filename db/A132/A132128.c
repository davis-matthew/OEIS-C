/*
 * A132128  A051340 + A000027 - A000012.
 * https://oeis.org/A132128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = n + k - 1 if n = k(k+1)/2 for some integer k ≥ 1,
 *            otherwise a(n) = n. The triangular indices are increased by k-1
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   int_sqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_sqrt128.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for index 0, not used */
    unsigned __int128 val = (unsigned __int128)1 + (unsigned __int128)8 * n;
    unsigned __int128 s = int_sqrt128(val);

    seq_t inc = 0;
    if (s * s == val && (s & 1) == 1) {          /* n is triangular */
        unsigned __int128 k = (s - 1) / 2;        /* k such that n = k(k+1)/2 */
        inc = (seq_t)(k - 1);
    }
    return (seq_t)n + inc;
}
