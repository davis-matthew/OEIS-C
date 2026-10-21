/*
 * A217831  Euclid's triangle read by rows. T(n, k) = 1 if k is prime to n,
 *          otherwise 0.
 * https://oeis.org/A217831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  core, nice, easy, nonn, tabl
 * Formula:   a(n) = 1 iff gcd(row, column) = 1, * where row = ⌊(√(8n+1) –
 *            1)/2⌋ and column = n – row(row+1)/2. * T(0,0) = 0; all other
 *            entries are 1 exactly when the two indices are coprime
 * Range:     u8 n <= 9710, u16 n <= 9710, u32 n <= 9710, u64 n <= 9710,
 *            u128 n <= 9710, big any n
 * Helpers:   uint64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/uint64_gcd.h"

/* Greatest‑common divisor for 64‑bit indices. */

/* Integer square root of a 128‑bit unsigned value. */
static inline idx_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = (((unsigned __int128)1) << 64) - 1;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == x)
            return (idx_t)mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (idx_t)(low - 1);  /* floor(sqrt(x)) */
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    unsigned __int128 t = (unsigned __int128)n * 8 + 1;
    idx_t      s = isqrt_u128(t);
    idx_t      row = (s - 1) / 2;

    unsigned __int128 r = row;
    unsigned __int128 prev = r * (r + 1) / 2;
    idx_t      col = n - (idx_t)prev;

    if (row == 0 && col == 0)          /* T(0,0) = 0 */
        return (seq_t)0;

    idx_t g = uint64_gcd(row, col);
    return (seq_t)(g == 1 ? 1 : 0);
}
