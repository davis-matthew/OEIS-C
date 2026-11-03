/*
 * A169335  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 34 * 33^(n-1) for n > 0, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static inline seq_t seq_one(void) { return (seq_t)1; }

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
    if (n == 0)
        return seq_one();          /* a(0) = 1 */

    seq_t pow = seq_one();          /* start with 33^0 = 1 */
    /* compute 33^(n-1) by repeated multiplication   */
    for (idx_t i = 0; i < n - 1; ++i)
        pow = mul_small(pow, 33);

    /* multiply by 34 to get the final result          */
    return mul_small(pow, 34);
}
