/*
 * A159401  Number of n X n arrays of squares of integers summing to 18.
 * https://oeis.org/A159401
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum over all (c1,c4,c9,c16) with c1+4c4+9c9+16c16 = 18 of
 *            * binom(n², c1) * binom(n²‑c1, c4) * binom(n²‑c1‑c4, c9) * *
 *            binom(n²‑c1‑c4‑c9, c16) * (counting n×n arrays of squares of
 *            integers whose entries sum to 18)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

typedef struct {
    idx_t c1, c4, c9, c16;
} sol_t;

static const sol_t solutions[] = {
    {18,0,0,0},
    {14,1,0,0},
    {10,2,0,0},
    {6,3,0,0},
    {2,4,0,0},
    {9,0,1,0},
    {5,1,1,0},
    {1,2,1,0},
    {0,0,2,0},
    {2,0,0,1}
};

static const idx_t SOL_COUNT = sizeof(solutions)/sizeof(solutions[0]);

seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* offset is 2 */
    seq_t m = (seq_t)n * (seq_t)n;     /* m = n² */
    seq_t total = 0;
    for (idx_t i = 0; i < SOL_COUNT; ++i) {
        const sol_t *s = &solutions[i];
        /* skip solutions that need more cells than available */
        if (s->c1 + s->c4 + s->c9 + s->c16 > (idx_t)m)
            continue;
        seq_t t = binom((idx_t)m, s->c1);
        t *= binom((idx_t)(m - s->c1), s->c4);
        t *= binom((idx_t)(m - s->c1 - s->c4), s->c9);
        t *= binom((idx_t)(m - s->c1 - s->c4 - s->c9), s->c16);
        total += t;
    }
    return total;
}
