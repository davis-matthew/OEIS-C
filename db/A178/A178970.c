/*
 * A178970  a(n) = 2^(2n-1) mod (2n+1).
 * https://oeis.org/A178970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2n-1) mod (2n+1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe guard for calls below offset */
    seq_t mod = ((seq_t)n << 1) + 1;/* 2n + 1 */
    uint64_t exp = (n << 1) - 1;    /* 2n - 1 */
    return powmod((seq_t)2, exp, mod);
}
