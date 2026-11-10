/*
 * A085208  Transpose of A085207.
 * https://oeis.org/A085208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = concat(bin(x), bin(y)) with (x,y) = (k, s-k), where s =
 *            floor((sqrt(8n+1)-1)/2), k = n - s(s+1)/2; zero is the empty
 *            string (transpose of A085207)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-07)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/isqrt_u128.h"

static size_t A085208_bitlen(idx_t v) {
    size_t len = 0;
    while (v) {
        v >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n) {
    unsigned __int128 sq = isqrt_u128((unsigned __int128)n * 8 + 1);
    idx_t s = (idx_t)((sq - 1) / 2);
    idx_t diag_start = (idx_t)((unsigned __int128)s * (s + 1) / 2);
    idx_t offset = n - diag_start;
    idx_t x = offset;
    idx_t y = s - offset;
    size_t leny = A085208_bitlen(y);
    return (((seq_t)x) << leny) | ((seq_t)y);
}
