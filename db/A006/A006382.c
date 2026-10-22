/*
 * A006382  Number of n X 4 binary matrices under row and column permutations
 *          and column complementations.
 * https://oeis.org/A006382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: (1/(1-x)^16 + 51/(1-x^2)^8 + 12/((1-x)^8*(1-x^2)^4) +
 *            84/(1-x^4)^4 + 12/((1-x)^4*(1-x^2)^6) + 32/((1-x)^4*(1-x^3)^4) +
 *            96/((1-x^2)^2*(1-x^6)^2) + 48/((1-x)^2*(1-x^2)*(1-x^4)^3) +
 *            48/(1-x^8)^2)/384. Coefficients are kept as 384*q + r (0 <= r <
 *            384) so nothing exceeds about a(n)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 34, u64 n <= 175,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

/* Adds the coefficient of x^n in m/prod(1-x^f)^p to (*qs, *rs), where the
   coefficient is stored as 384*q + r. */
static void add_term(idx_t n, const idx_t *factors, const idx_t *powers, idx_t nf,
                     unsigned m, seq_t *qs, unsigned *rs) {
    seq_t *q = alloc_zero(n + 1);
    unsigned *r = (unsigned *)calloc(n + 1, sizeof(unsigned));
    r[0] = m;
    for (idx_t f = 0; f < nf; f++) {
        idx_t s = factors[f];
        for (idx_t p = 0; p < powers[f]; p++) {
            for (idx_t i = s; i <= n; i++) {
                q[i] += q[i - s];
                r[i] += r[i - s];
                if (r[i] >= 384) {
                    r[i] -= 384;
                    q[i] += 1;
                }
            }
        }
    }
    *qs += q[n];
    *rs += r[n];
    if (*rs >= 384) {
        *rs -= 384;
        *qs += 1;
    }
    release(q, n + 1);
    free(r);
}

seq_t a(idx_t n) {
    seq_t qs = 0;
    unsigned rs = 0;
    {
        idx_t f[] = {1}, p[] = {16};
        add_term(n, f, p, 1, 1, &qs, &rs);
    }
    {
        idx_t f[] = {2}, p[] = {8};
        add_term(n, f, p, 1, 51, &qs, &rs);
    }
    {
        idx_t f[] = {1, 2}, p[] = {8, 4};
        add_term(n, f, p, 2, 12, &qs, &rs);
    }
    {
        idx_t f[] = {4}, p[] = {4};
        add_term(n, f, p, 1, 84, &qs, &rs);
    }
    {
        idx_t f[] = {1, 2}, p[] = {4, 6};
        add_term(n, f, p, 2, 12, &qs, &rs);
    }
    {
        idx_t f[] = {1, 3}, p[] = {4, 4};
        add_term(n, f, p, 2, 32, &qs, &rs);
    }
    {
        idx_t f[] = {2, 6}, p[] = {2, 2};
        add_term(n, f, p, 2, 96, &qs, &rs);
    }
    {
        idx_t f[] = {1, 2, 4}, p[] = {2, 1, 3};
        add_term(n, f, p, 3, 48, &qs, &rs);
    }
    {
        idx_t f[] = {8}, p[] = {2};
        add_term(n, f, p, 1, 48, &qs, &rs);
    }
    /* the total is a multiple of 384, so rs ends at 0 */
    return qs;
}
