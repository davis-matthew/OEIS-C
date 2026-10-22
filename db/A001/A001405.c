/*
 * A001405  a(n) = binomial(n, floor(n/2)).
 * https://oeis.org/A001405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core, walk, changed
 * Formula:   a(n) = binomial(n, floor(n/2)) = Product_{i=1..floor(n/2)}
 *            (n-floor(n/2)+i)/i
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   a001405_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001405_gcd_u64.h"

seq_t a(idx_t n)
{
    idx_t k = n / 2;
    idx_t base = n - k;
    seq_t result = 1;
    idx_t i;

    for (i = 1; i <= k; i++) {
        /* result * (base + i) / i is exact; divide out the common factor
           first so no intermediate value exceeds the new result */
        uint64_t num = (uint64_t)(base + i);
        uint64_t den = (uint64_t)i;
        uint64_t g = a001405_gcd_u64(num, den);
        num /= g;
        den /= g;
        /* den is coprime to num, so den divides result */
        result = (result / (seq_t)den) * (seq_t)num;
    }

    return result;
}
