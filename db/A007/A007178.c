/*
 * A007178  Number of ways to write 1 as ordered sum of n powers of 1/2,
 *          allowing repeats.
 * https://oeis.org/A007178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = g(n,1), where g(r,s) counts ordered ways to fill s slots
 *            of the current level of powers of 1/2 using exactly r terms.
 *            Choose c slots to hold terms; each other slot splits into 2
 *            slots at the next level: g(r,s) = Sum_{c=0..min(s,r)}
 *            binomial(r,c) * g(r-c, 2*(s-c)), g(0,0) = 1, g(r,0) = 0 fo...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"

static seq_t **H = NULL;   /* H[r][t] = g(r, 2t), t = 0..r/2 */
static seq_t **B = NULL;   /* B[r][k] = binomial(r, k) */
static idx_t done = 0;     /* rows 0..done-1 computed */
static idx_t cap = 0;

static void ensure(idx_t n) {
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < n + 1) nc *= 2;
        H = (seq_t **)realloc(H, (size_t)nc * sizeof(seq_t *));
        B = (seq_t **)realloc(B, (size_t)nc * sizeof(seq_t *));
        cap = nc;
    }
    for (idx_t r = done; r <= n; r++) {
        B[r] = alloc_zero(r + 1);
        B[r][0] = 1;
        B[r][r] = 1;
        for (idx_t k = 1; k < r; k++)
            B[r][k] = B[r - 1][k - 1] + B[r - 1][k];

        idx_t half = r / 2;
        H[r] = alloc_zero(half + 1);
        H[r][0] = (r == 0) ? 1 : 0;
        for (idx_t t = half; t >= 1; t--) {
            seq_t sum = 0;
            idx_t clo = (4 * t > r) ? 4 * t - r : 0;
            idx_t chi = 2 * t;
            for (idx_t c = clo; c <= chi; c++) {
                idx_t rr = r - c;
                idx_t tt = 2 * t - c;
                sum += B[r][c] * H[rr][tt];
            }
            H[r][t] = sum;
        }
    }
    if (n + 1 > done) done = n + 1;
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    ensure(n);
    return H[n][1];
}
