/*
 * A076569  Greatest prime divisor of 6n+15 (sum of six successive integers).
 * https://oeis.org/A076569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = greatest prime divisor of (6*n + 15) Since 6*n + 15 = 3 *
 *            (2*n + 5) and 2*n + 5 is odd, the result is the greatest prime
 *            factor of (2*n + 5)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    /* 2*n + 5 fits in 64 bits for all practical n (the tests use n
       up to at least 10 000).  Perform the multiplication in 128‑bit
       to avoid overflow before casting back to 64 bits. */
    uint64_t m = (uint64_t)(((unsigned __int128)n) * 2 + 5);
    /* gpf returns the greatest prime factor of a 64‑bit integer. */
    return (seq_t)gpf(m);
}
