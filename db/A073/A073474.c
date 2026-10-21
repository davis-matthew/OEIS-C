/*
 * A073474  Triangle T(n,k) read by rows, where o.g.f. for T(n,k) is
 *          n!*Sum_{k=0..n} (1+x)^(n-k)/k!.
 * https://oeis.org/A073474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k), where * r = ⌊(√(1+8n) – 1)/2⌋ (row of the
 *            triangle) * k = n – r(r+1)/2 (column in that row) * T(r,k) =
 *            Σ_{j=k..r} binomial(j,k) · (r)_j with (r)_j = r·(r–1)…(r–j+1)
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 67, u64 n <= 174,
 *            u128 n <= 445, big any n
 * Helpers:   binom_ck
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_ck.h"

/* Integer square root for 64‑bit unsigned values.
 * Uses a simple bit‑shift binary search and avoids overflow with __int128. */
static idx_t int_sqrt(idx_t x) {
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 62;          /* start with the highest bit <= sqrt(2^64-1) */
    while (bit) {
        idx_t next = r | bit;
        if ((__int128)next * next <= x)
            r = next;
        bit >>= 1;
    }
    return r;
}

/* Small integer r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
static idx_t row_of(idx_t n) {
    idx_t t = 8 * n + 1;
    idx_t s = int_sqrt(t);                 /* floor sqrt(1+8n) */
    return (s - 1) / 2;
}

/* Return the n‑th element of OEIS sequence A073474 (flattened triangle). */
seq_t a(idx_t n) {
    /* Find row r and column k corresponding to index n. */
    idx_t r = row_of(n);
    idx_t before = r * (r + 1) / 2;
    idx_t k = n - before;

    /* Compute T(r,k) = Σ_{j=k..r} binomial(j,k) * (r)_j. */
    seq_t result = 0;
    seq_t falling = 1;          /* (r)_0 */

    for (idx_t j = 0; j <= r; ++j) {
        if (j >= k) {
            seq_t b = binom_ck(j, k);
            result += b * falling;
        }
        if (j < r) {           /* update falling factorial for next j */
            falling *= (seq_t)(r - j);
        }
    }
    return result;
}
