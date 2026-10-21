/*
 * A060372  p(n), positive part of n when n=p(n)-q(n) with p(n), q(n),
 *          p(n)+q(n) in A005836, integers written without 2 in base 3.
 * https://oeis.org/A060372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = the smallest positive part p of n such that there exist q
 *            ∈ A005836 with p−q = n and p+q ∈ A005836. Implementation:
 *            iterate over s = p+q (s ∈ A005836, s ≥ n) until both p = (s+n)/2
 *            and q = (s−n)/2 are integers in A005836. Then return p
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   base3_only_01
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/base3_only_01.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t s = nn;
    for (;;)
    {
        if (base3_only_01(s) && ((s + nn) & 1) == 0)
        {
            seq_t p = (s + nn) >> 1;
            seq_t q = (s - nn) >> 1;
            if (p >= q && base3_only_01(p) && base3_only_01(q))
                return p;
        }
        ++s;
    }
}
