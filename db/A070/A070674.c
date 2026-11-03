/*
 * A070674  Smallest m in range 2..n-1 such that m^9 == 1 mod n, or 1 if no
 *          such number exists.
 * https://oeis.org/A070674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 384, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9672 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest m in [2..n-1] such that m^9 ≡ 1 (mod n), or 1 if no such m. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    seq_t mod = (seq_t)n;
    for (idx_t m = 2; m <= n - 1; ++m)
    {
        if (powmod((seq_t)m, 9, mod) == (seq_t)1)
            return (seq_t)m;
    }
    return (seq_t)1;
}
