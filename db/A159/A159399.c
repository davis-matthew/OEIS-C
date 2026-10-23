/*
 * A159399  Number of n X n arrays of squares of integers summing to 17.
 * https://oeis.org/A159399
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of n×n arrays of integers whose squares sum to 17
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 36,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

struct combo { int a, b, c, d; };

/* All distinct quadruples (a,b,c,d) such that 16a+9b+4c+d = 17. */
static const struct combo combos[] = {
    {1,0,0,1},
    {0,1,0,8},
    {0,1,1,4},
    {0,1,2,0},
    {0,0,0,17},
    {0,0,1,13},
    {0,0,2,9},
    {0,0,3,5},
    {0,0,4,1}
};
static const size_t combos_cnt = sizeof(combos)/sizeof(combos[0]);

/* Binomial coefficient C(n,k) for small k (k ≤ 17).  This routine uses
   GCD‑cancellation to keep intermediate values small and avoid overflow
   when seq_t is a fixed‑size integer type. */
static seq_t binom_small(seq_t n, int k)
{
    if (k == 0) return (seq_t)1;
    int i;
    seq_t numer[18], denom[18];          /* k ≤ 17, so room for 18 */
    for (i = 0; i < k; ++i) numer[i] = n - (seq_t)i;
    for (i = 0; i < k; ++i) denom[i] = (seq_t)(i + 1);

    /* Cancel common factors between numerators and denominators. */
    for (i = 0; i < k; ++i) {
        if (denom[i] == 1) continue;
        for (int j = 0; j < k; ++j) {
            seq_t g = gcd(denom[i], numer[j]);
            if (g > 1) {
                denom[i] /= g;
                numer[j] /= g;
                if (denom[i] == 1) break;
            }
        }
    }

    seq_t res = (seq_t)1;
    for (i = 0; i < k; ++i) res *= numer[i];
    return res;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2. */
    if (n < 2) return (seq_t)0;

    seq_t m = (seq_t)n * (seq_t)n;        /* number of cells in the array */
    seq_t result = (seq_t)0;

    for (size_t idx = 0; idx < combos_cnt; ++idx) {
        const struct combo *c = &combos[idx];
        int sum_vars = c->a + c->b + c->c + c->d;   /* non‑zero entries used */

        if (m < (seq_t)sum_vars) continue;          /* not enough cells */

        seq_t term = binom_small(m, c->a);
        seq_t rem  = m - (seq_t)c->a;
        term *= binom_small(rem, c->b);
        rem  -= (seq_t)c->b;
        term *= binom_small(rem, c->c);
        rem  -= (seq_t)c->c;
        term *= binom_small(rem, c->d);

        result += term;
    }

    return result;
}
