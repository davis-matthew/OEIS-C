/*
 * A051194  Triangular array T read by rows: T(n,k) = number of positive
 *          integers that divide both n and k.
 * https://oeis.org/A051194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = d(gcd(r,c)) where r is the row index, c is the column
 *            index in the triangular arrangement: r = smallest integer with
 *            r(r+1)/2 >= n, c = n - (r-1)*r/2 (1 <= c <= r). d(x) is the
 *            divisor count of x
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    /* Find row r (1‑based) such that r(r+1)/2 >= n */
    uint64_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) high <<= 1;
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    uint64_t r = low;

    /* Column index c (1‑based) */
    uint64_t prev_tri = (unsigned __int128)(r - 1) * r / 2;
    uint64_t c = n - prev_tri;

    /* gcd of r and c, both < 2^64 */
    uint64_t g = (uint64_t)gcd((seq_t)r, (seq_t)c);

    /* divisor count of g */
    uint64_t d = divisor_count_u64(g);

    return (seq_t)d;
}
