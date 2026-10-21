/*
 * A216455  If n is a perfect fourth power, then replace it with the fourth
 *          root of n.
 * https://oeis.org/A216455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n, unless n is a perfect fourth power k^4, then a(n)=k
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   int_fourth_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_fourth_root.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety for out‑of‑range calls */
    idx_t k = int_fourth_root(n);
    uint64_t k2 = (uint64_t)k * k;
    uint64_t k4 = k2 * k2;
    if (k4 == n)
        return (seq_t)k;
    else
        return (seq_t)n;
}
