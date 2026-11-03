/*
 * A022167  Triangle of Gaussian binomial coefficients [ n,k ] for q = 3.
 * https://oeis.org/A022167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 48, u64 n <= 95, u128 n <= 179,
 *            big any n
 * Validated: standard, pass, 61 terms through n = 1138 (2026-11-03)
 * Helpers:   mem, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute OEIS sequence A022167: 
 * Triangle of Gaussian binomial coefficients [n,k] for q = 3,
 * enumerated row‑wise with offset 0.
 * The value at index n is the q‑binomial coefficient
 * C_q(n,k) = [n,k]_3 with k derived from the triangular index.
 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow3.h"
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Determine row r and column k from the triangular index n.
       row r has length r+1.  The cumulative number of elements up to
       row r-1 equals r*(r+1)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;
    idx_t k = n - r * (r + 1) / 2;   /* 0 <= k <= r */

    /* Base cases */
    if (r == 0)
        return (seq_t)1;

    /* Pre‑compute powers of 3 up to 3^r (pow3[0] = 1) */
    seq_t *pow3 = mem_new(seq_t, (size_t)(r + 1));
    if (!pow3) return (seq_t)0; /* allocation failure */
    pow3[0] = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i)
        pow3[i] = mul_small(pow3[i - 1], 3);

    /* Allocate two rows for dynamic programming */
    seq_t *row0 = mem_new(seq_t, (size_t)(r + 1));
    seq_t *row1 = mem_new(seq_t, (size_t)(r + 1));
    if (!row0 || !row1) {
        mem_free(row0); mem_free(row1); mem_free(pow3);
        return (seq_t)0; /* allocation failure */
    }

    /* Initialize first row (r = 0) */
    row0[0] = (seq_t)1;

    /* Build rows iteratively up to r */
    for (idx_t i = 1; i <= r; ++i) {
        row1[0] = (seq_t)1;       /* first element of each row is 1 */
        for (idx_t j = 1; j < i; ++j) {
            seq_t prod = row0[j] * pow3[j];
            row1[j] = row0[j - 1] + prod;
        }
        row1[i] = (seq_t)1;       /* last element of each row is 1 */

        /* swap rows for next iteration */
        seq_t *tmp = row0;
        row0 = row1;
        row1 = tmp;
    }

    /* Result is the k-th element of the last computed row */
    seq_t result = row0[k];

    /* Clean up */
    mem_free(row0);
    mem_free(row1);
    mem_free(pow3);
    return result;
}
