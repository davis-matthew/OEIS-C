/*
 * A028425  Clog sequence in base 4. Right to left concatenation of n,
 *          int(log_4(n)), int(log_4(int(log_4(n)))), ... in base 4.
 * https://oeis.org/A028425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   * a(n) = base‑4 concatenation of n, floor(log₄(n)),
 *            floor(log₄(floor(log₄(n)))), ..., * interpreted as a decimal
 *            integer. The concatenation is performed from right to * left:
 *            the leftmost part is the last log term (0 unless n=1). *
 *            Example: n = 4 → base4("10"), log₄(4)=1 → base4("1") → "1 10" →
 *            v...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   base4_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/base4_len.h"

/* Helper: floor(log₂(v)) for 1 ≤ v < 2⁶⁴ */
static inline idx_t floor_log2_u64(idx_t v) {
    return 63 - __builtin_clzll(v);
}

/* Helper: floor(log₄(v)) = floor(log₂(v)) / 2 */
static inline idx_t floor_log4_u64(idx_t v) {
    return floor_log2_u64(v) >> 1;
}

/* Compute power 4ⁿ for small n */
static inline seq_t pow4_u64(size_t n) {
    seq_t r = 1;
    while (n--) r *= 4;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* not needed for OEIS offset, but safe */
    seq_t cur = (seq_t)n;               /* start with n itself */
    idx_t prev = n;                     /* next log is taken of this value */

    while (true) {
        idx_t val = floor_log4_u64(prev); /* floor(log₄(prev))   */
        if (val == 0) break;             /* adding 0 changes nothing */

        /* length of current base‑4 representation */
        size_t len = base4_len(cur);

        /* shift cur left by len base‑4 digits: multiply by 4^len */
        seq_t shift = pow4_u64(len);

        cur = (seq_t)val * shift + cur; /* prepend val */
        prev = val;                     /* next log is of this value */
    }
    return cur;
}
