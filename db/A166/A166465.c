/*
 * A166465  a(n) = 3*a(n-2) for n > 2; a(1) = 1, a(2) = 5.
 * https://oeis.org/A166465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 79, u128 n <= 161,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 986 (2026-11-01)
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=5, a(n)=3*a(n-2) for n>2 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow3.h"

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
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    /* odd indices: a(2k+1) = 3^k,  k = (n-1)/2 */
    if ((n & 1) != 0)
    {
        idx_t k = (n - 1) >> 1;
        return pow3(k);
    }

    /* even indices: a(2k) = 5 * 3^(k-1), k = n/2 */
    idx_t k = n >> 1;
    seq_t pow_val = pow3(k - 1);
    return mul_small(pow_val, (unsigned)5);
}
