/*
 * A185782  Weight array of A185780, by antidiagonals.
 * https://oeis.org/A185782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 if n is a triangular number T_k with k ≥ 2; else a(n) =
 *            2k+1 if n = T_k + 1, else a(n) = 2k – 2j where j = n – T_k – 1,
 *            0 < j < k. Here T_k = k(k+1)/2. For n = 1, a(1) = 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static inline uint64_t floor_isqrt_u64(uint64_t x) {
    if (x == 0 || x == 1) return x;
    /* Use double sqrt for speed; adjust to exact floor. */
    double d = (double)x;
    uint64_t r = (uint64_t) sqrt(d);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    /* Compute k = floor( (sqrt(8 n + 1) - 1) / 2 ) */
    uint64_t d = 8 * n + 1;
    uint64_t s = floor_isqrt_u64(d);
    uint64_t k = (s - 1) / 2;

    /* Triangular number T_k */
    uint64_t t = k * (k + 1) / 2;

    if (n == t && k >= 2) return (seq_t)0;   /* zeros at triangular indices ≥ T_2 */

    uint64_t j = n - t - 1;   /* position in block: 0‑based */

    if (j == 0)
        return (seq_t)(2 * k + 1);
    else
        return (seq_t)(2 * k - 2 * j);
}
