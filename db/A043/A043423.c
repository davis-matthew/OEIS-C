/*
 * A043423  Numbers having three 0's in base 8.
 * https://oeis.org/A043423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 693, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   zeros_in_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth integer that has exactly three 0's in its base‑8 representation. */
#include <stdint.h>
#include "../utils/zeros_in_base8.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset handling – unused but safe */
    seq_t cur = 512;                     /* first number with three 0's in base‑8 */
    idx_t cnt = 1;                       /* we have seen one such number */

    if (n == 1) return cur;

    for (cur = cur + 1u; ; ++cur) {
        if (zeros_in_base8(cur) == 3u) {
            ++cnt;
            if (cnt == n) return cur;
        }
    }
}
