/*
 * A015948  a(n) = smallest k >= n such that k | (2^k + n).
 * https://oeis.org/A015948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ n+1 such that k | (2^k + (n+1))
 * Range:     u8 n <= 21, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = min{ k ≥ n+1 | 2^k ≡ –(n+1) (mod k) } */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    seq_t start = (seq_t)n + 1;          /* first k to try */
    for (seq_t k = start; ; ++k) {
        seq_t pow2 = powmod((seq_t)2, (uint64_t)k, k);  /* 2^k mod k */
        seq_t sum  = pow2 + ((seq_t)n + 1);              /* 2^k + (n+1) */
        if (sum >= k) sum -= k;                         /* reduce mod k */
        if (sum == 0) return k;                         /* found the smallest k */
    }
}
