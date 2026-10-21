/*
 * A070501  a(n) = n^3 mod 39.
 * https://oeis.org/A070501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 39
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* n.mod_39  -> small values 0..38 */
    uint64_t mod = 39;
    uint64_t nm = n % mod;                /* n % 39  */
    uint64_t sq = (nm * nm) % mod;        /* (n^2) % 39 */
    uint64_t cub = (sq * nm) % mod;       /* (n^3) % 39 */
    return (seq_t)cub;
}
