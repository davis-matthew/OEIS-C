/*
 * A104734  Triangle T(n,k) = sum_{j=k..n} (2n-2j+1)*binomial(k,j-k), read by
 *          rows, 0<=k<=n.
 * https://oeis.org/A104734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute sequence A104734.
 *
 * Triangle entries:
 *   T(n,k) = sum_{j=k..n} (2n-2j+1) * binomial(k, j-k),     0 ≤ k ≤ n.
 *
 * The sequence a(n) is obtained by reading T row‑by‑row (row‑major order).
 *
 * a(n) is defined for n ≥ 0 and has the first few values:
 *   1, 3, 1, 5, 4, 1, 7, 8, 5, 1, 9, 12, 12, 6, 1, …
 *
 * Author: ChatGPT
 */

#include <stdint.h>

static inline idx_t isqrt(idx_t x) {
    /* Integer square root: floor(sqrt(x)). */
    idx_t res = 0;
    idx_t bit = 1ULL << 62;          /* largest power of four ≤ 2⁶⁴ */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Determine row and column from the flattened index n. */
    idx_t tmp = (idx_t)((__uint128_t)n * 8 + 1);
    idx_t s   = isqrt(tmp);               /* floor(sqrt(8n+1)) */
    idx_t row = (s - 1) / 2;              /* row index */
    idx_t prev = (idx_t)((__uint128_t)row * (row + 1) / 2);  /* C(row-1) */
    idx_t col  = n - prev;                /* column within the row */

    /* Compute T(row, col). */
    idx_t m = row - col;                  /* j-k ranges from 0 to m */
    seq_t base = (seq_t)((seq_t)row * 2 - (seq_t)col * 2 + 1); /* 2n-2k+1 */
    seq_t sum  = 0;
    seq_t bin  = 1;                        /* binomial(col, 0) */

    /* limit the loop to r <= min(m, col) to avoid negative numerators. */
    idx_t limit = m < col ? m : col;
    for (idx_t r = 0; r <= limit; ++r) {
        seq_t coeff = base - (seq_t)(2 * r);
        sum += coeff * bin;
        if (r < limit) {
            bin = bin * (seq_t)(col - r) / (seq_t)(r + 1);
        }
    }

    return sum;
}
