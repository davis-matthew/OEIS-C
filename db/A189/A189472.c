/*
 * A189472  n+[ns/r]+[nt/r]; r=1, s=e/2, t=2/e.
 * https://oeis.org/A189472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n * e / 2⌋ + ⌊2 * n / e⌋ (r = 1, s = e/2, t = 2/e)
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t SHIFT   = 1000000000000ULL;   /* 10^12 – scaling factor   */
static const uint64_t ESCALE  = 2718281828459ULL;   /* floor(e * SHIFT)         */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* outside the OEIS offset – harmless */

    /* ⌊n * e / 2⌋  →  ⌊ n * ESCALE / (2 * SHIFT) ⌋ */
    __uint128_t t1   = (__uint128_t)n * ESCALE;
    seq_t      term1 = (seq_t)(t1 / (2 * SHIFT));

    /* ⌊2 * n / e⌋  →  ⌊ 2 * n * SHIFT / ESCALE ⌋ */
    __uint128_t t2   = (__uint128_t)2 * n * SHIFT;
    seq_t      term2 = (seq_t)(t2 / ESCALE);

    return (seq_t)n + term1 + term2;
}
