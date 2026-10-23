/*
 * A008754  Expansion of (1+x^11)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A008754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_3(n) + p_3(n-11) for n ≥ 11, a(n) = p_3(n) for n < 11,
 *            where p_3(m) is the number of partitions of m into parts 1, 2,
 *            3. p_3(m) = floor((m*m + 6*m + 12)/12) for m ≥ 0. The
 *            computation uses 64‑bit arithmetic for the intermediate
 *            arithmetic to avoid overflow on small seq_t types
 * Range:     u8 n <= 41, u16 n <= 629, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* only for uint64_t (used for intermediates) */
#include "../utils/part.h"

seq_t a(idx_t n)
{
    if (n < 11) {
        return part(n);
    } else {
        return part(n) + part(n - 11);
    }
}
