/*
 * A003956  Order of complex Clifford group of degree 2^n arising in quantum
 *          coding theory.
 * https://oeis.org/A003956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^(n^2+2*n+3) * Product_{k=1..n} (4^k-1) computed as a(n)
 *            = 8 * Product_{k=1..n} 2^(2k+1) * (2^(2k) - 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

static seq_t shl_safe(seq_t x, idx_t s) {
    if (!seq_is_fixed_width()) {
        x <<= s;
        return x;
    }
    {
        idx_t chunk = (idx_t)(8 * sizeof(seq_t) - 1);
        while (s > 0) {
            idx_t t = s > chunk ? chunk : s;
            if (x == 0) return x;
            x <<= t;
            s -= t;
        }
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t x = 8;
    idx_t k;
    for (k = 1; k <= n; k++) {
        /* x *= (4^k - 1) as (x << 2k) - x */
        x = shl_safe(x, 2 * k) - x;
        /* x *= 2^(2k+1) */
        x = shl_safe(x, 2 * k + 1);
        if (seq_is_fixed_width() && x == 0) break;
    }
    return x;
}
