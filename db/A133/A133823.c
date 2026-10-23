/*
 * A133823  Triangle whose rows are sequences of increasing and decreasing
 *          cubes:1; 1,8,1; 1,8,27,8,1; ... .
 * https://oeis.org/A133823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = (1 + min(n - ⌊√n⌋², 2·⌊√n⌋ - (n - ⌊√n⌋²)))³
 * Range:     u8 n <= 41, u16 n <= 1639, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where ⌊√n⌋ is the floor of the square root of n.
    The sequence is the cube of the known A004737 sequence. */

#include <stdint.h>
#include <limits.h>

/* Integer square root: floor(sqrt(n)) for idx_t (uint64_t) */
static inline idx_t isqrt(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= (unsigned __int128)n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t r = isqrt(n);                /* floor sqrt(n) */
    idx_t pos = n - r * r;             /* position inside the row */
    idx_t two_r = r << 1;              /* 2 * r */
    idx_t k = pos < (two_r - pos) ? pos : (two_r - pos);
    seq_t v = (seq_t)1 + (seq_t)k;     /* the base value before cubing   */
    seq_t val = v * v * v;             /* compute the cube                 */
    return val;
}
