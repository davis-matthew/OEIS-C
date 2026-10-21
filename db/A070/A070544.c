/*
 * A070544  Number of squarefree numbers s such that n < s < 2n.
 * https://oeis.org/A070544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=n+1}^{2n-1} μ(k)^2 (μ is the Möbius function)
 * Range:     u8 n <= 415, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   squarefree_in_interval
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/squarefree_in_interval.h"

/* Public entry point required by the test harness */
seq_t a(idx_t n)
{
    return squarefree_in_interval(n);
}
