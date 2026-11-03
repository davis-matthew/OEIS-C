/*
 * A171553  a(n) = 9*10^n + 100^n + 1.
 * https://oeis.org/A171553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*10^n + 100^n + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 96 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_u64_idx.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Compute 10^n and 100^n using the provided utility */
    seq_t p10  = ipow_u64_idx(10ULL, n);
    seq_t p100 = ipow_u64_idx(100ULL, n);

    /* Compute the sequence value from the formula */
    seq_t result = mul_small(p10, 9U); /* 9 * 10^n   */
    result += p100;                   /* + 100^n    */
    result += 1U;                     /* + 1        */

    return result;
}
