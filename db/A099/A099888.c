/*
 * A099888  XOR BINOMIAL transform of the powers of 3.
 * https://oeis.org/A099888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = XOR_{k subset of n} 3^k; i.e. a(n) = ⊕_{k⊆n} 3^k
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (k runs over all submasks of n, including 0) */

#include <stdbool.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t sub = n; ; sub = (sub - 1) & n)
    {
        res ^= pow3(sub);
        if (sub == 0)
            break;
    }
    return res;
}
