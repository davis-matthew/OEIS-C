/*
 * A165186  a(n) = Sum_{k=1..n} (k*(n-k) mod n).
 * https://oeis.org/A165186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k*(n-k) mod n)
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k) {
        /* use 128‑bit integer for the product to avoid overflow on small seq_t types */
        unsigned __int128 prod = (unsigned __int128)k * (unsigned __int128)(n - k);
        seq_t term = (seq_t)(prod % (unsigned __int128)n);
        sum += term;
    }
    return sum;
}
