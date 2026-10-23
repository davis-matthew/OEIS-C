/*
 * A194167  [sum{(k*r) : 1<=k<=n}], where [ ]=floor, ( )=fractional part, and
 *          r=(3+sqrt(5))/2, the golden ratio.
 * https://oeis.org/A194167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊∑_{k=1}^n {k·√5/2}⌋ where {x} denotes the fractional
 *            part of x. This is equivalent to a(n) = ⌊⌊√5·n(n+1)/4⌋⌋ –
 *            Σ_{k=1}^n ⌊⌊√5·k⌋/2⌋ The implementation uses only integer
 *            arithmetic and an integer square‑root routine. It works for any
 *            idx_t ≥ 1
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer (floor). */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    if (x == 0) return 0;
    int h = 63 - __builtin_clzll(x);          /* highest set bit index   */
    uint64_t r = 1ULL << (h >> 1);            /* initial guess           */
    /* refine the guess.  Using 128‑bit arithmetic for safety. */
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) ++r;
    while ((unsigned __int128)r * r > x) --r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safe for n=0, though the offset is 1 */

    uint64_t N = (uint64_t)n;
    uint64_t T = N * (N + 1) / 2;        /* triangular number n(n+1)/2 */

    /* part1 = ⌊⌊√5·T⌋ / 2⌋ = ⌊⌊√5·n(n+1)/4⌋⌋  */
    uint64_t sqrt_T = isqrt_uint64((unsigned __int128)5 * T * T);
    uint64_t part1  = sqrt_T >> 1;           /* divide by 2 (floor) */

    uint64_t sum = 0;
    for (uint64_t k = 1; k <= N; ++k)
    {
        uint64_t sqrt_k = isqrt_uint64((unsigned __int128)5 * k * k);
        sum += (sqrt_k >> 1);                /* floor(⌊√5·k⌋ / 2) */
    }

    return (seq_t)(part1 - sum);
}
