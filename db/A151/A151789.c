/*
 * A151789  a(1)=1; for n > 1, a(n)=5*4^{wt(n-1)-1}.
 * https://oeis.org/A151789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = 5 * 4^(wt(n-1)-1)
 * Range:     u8 n <= 15, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-01)
 * Helpers:   ipow_u64_idx, popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/popcount.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    /* Hamming weight of n-1 */
    int w = popcount(n - 1);
    /* compute 5 * 4^(w-1) */
    idx_t exp = (idx_t)(w > 0 ? w - 1 : 0);
    seq_t p = ipow_u64_idx(4ULL, exp);
    return (seq_t)5 * p;
}
