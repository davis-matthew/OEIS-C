/*
 * A161974  a(n) = number of equalities of the form A010060(n+k) = A010060(n),
 *          k=1,2,3.
 * https://oeis.org/A161974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{k = 1..3 : A010060(n+k) = A010060(n)}
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A010060 is the Thue–Morse sequence t(n) = parity(popcount(n)) */

#include <stdint.h>

/* parity of the number of 1-bits in an unsigned 64‑bit integer */
static inline unsigned popcount_parity(idx_t x)
{
    return __builtin_popcountll((unsigned long long)x) & 1U;
}

seq_t a(idx_t n)
{
    unsigned t   = popcount_parity(n);
    unsigned cnt = 0;

    if (popcount_parity(n + 1) == t) cnt++;
    if (popcount_parity(n + 2) == t) cnt++;
    if (popcount_parity(n + 3) == t) cnt++;

    return (seq_t)cnt;
}
