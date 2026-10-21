/*
 * A079114  Least squarefree number > n that is coprime to n.
 * https://oeis.org/A079114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least squarefree number > n that is coprime to n
 * (OEIS A079114).  We simply test successive candidates m > n
 * until we find one that is coprime to n and squarefree.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* index as seq_t for arithmetic */
    for (seq_t m = nn + (seq_t)1; ; ++m)
    {
        if (gcd(nn, m) == (seq_t)1 && is_squarefree(m))
            return m;
    }
}
