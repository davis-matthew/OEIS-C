/*
 * A156331  a(n)=8*A154811(n).
 * https://oeis.org/A156331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*A154811(n). The sequence has period 12. The *
 *            implementation computes the period‑12 values from a closed‑form
 *            mapping * rather than a lookup table. It works for any
 *            seq_t/idx_t types supplied * by the build
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   _map_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_map_k.h"

seq_t a(idx_t n)
{
    const idx_t n12 = n % 12ull;          /* period 12 */
    const idx_t k   = n12 % 6ull;         /* 0 .. 5 */
    seq_t val;
    if (n12 < 6ull)          /* first half of the period */
        val = _map_k(k);
    else                     /* second half is reverse of the first */
        val = _map_k(5 - k);
    return val * (seq_t)8;
}
