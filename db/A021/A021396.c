/*
 * A021396  Decimal expansion of 1/392.
 * https://oeis.org/A021396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit after the decimal point in 1/392. It is
 *            obtained by long division: starting remainder r = 1, then a(n) =
 *            (10·r)/392 and r <- (10·r) % 392
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    const uint64_t MOD = 392ULL;
    uint64_t r = 1;
    for (idx_t i = 0; i < n; ++i) {
        r = (10ULL * r) % MOD;
    }
    return (seq_t)((10ULL * r) / MOD);
}
