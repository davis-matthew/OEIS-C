/*
 * A178370  The trisection A178242(3n+2).
 * https://oeis.org/A178370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n*n + 27*n + 14) / (2 if n%4 is 0 or 1, else 4)
 * Range:     u8 n <= 7, u16 n <= 119, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute numerator in 128‑bit to avoid overflow for large n */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 num128 = 9ULL * N * N + 27ULL * N + 14ULL;
    uint64_t num = (uint64_t)num128;

    uint64_t res;
    if ((n & 3ULL) <= 1ULL) {
        /* denominator 2 */
        res = num >> 1;      /* same as num / 2 */
    } else {
        /* denominator 4 */
        res = num >> 2;      /* same as num / 4 */
    }

    return (seq_t)res;
}
