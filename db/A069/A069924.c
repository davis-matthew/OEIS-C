/*
 * A069924  Number of k, 1<=k<=n, such that phi(k) divides k.
 * https://oeis.org/A069924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{ k : 1 ≤ k ≤ n , φ(k) divides k }
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9638 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/totient.h"

/* seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* sequence is defined for n ≥ 1 */

    seq_t count = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t phi = totient(k);
        if ((seq_t)k % phi == 0)
            ++count;
    }

    return count;
}
