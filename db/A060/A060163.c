/*
 * A060163  a(n) = (n^3 + 5*n + 18)/6.
 * https://oeis.org/A060163
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^3 + 5*n + 18)/6
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Use signed 128‑bit arithmetic to avoid overflow while computing. */
    __int128 nn = (int64_t)n;
    __int128 val = (nn * nn * nn + (__int128)5 * nn + (__int128)18) / 6;
    return (seq_t)val;
}
