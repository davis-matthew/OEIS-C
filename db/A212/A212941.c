/*
 * A212941  Number of n X 6 0..5 arrays with no column j greater than or equal
 *          to than column j-1 in all rows.
 * https://oeis.org/A212941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * number of n×6 arrays over {0,…,5} such that for every
 *            j=2..6 * there exists at least one row i with a[i][j] <
 *            a[i][j-1]. * Computed by inclusion–exclusion over the 5
 *            adjacent–column constraints. * For S ⊆ {2,…,6} let f(S) be the
 *            number of 6‑tuples * (t1,…,t6) with t_j ≥ t_{j-1} for all j...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Utility: fast integer exponentiation for seq_t (works for 8/16/32/64/128 bits
 * and for arbitrary‑precision seq_t).                                   */
#include "../utils/pow_seq.h"

static seq_t f[32];          /* pre‑computed f(S) values, S encoded as bits 0..4
                                 (bit 0 → j=2, bit 4 → j=6). */
static unsigned int f_computed = 0;

/* Pre‑compute the 32 values of f(S). */
static void compute_f(void)
{
    int mask;
    for (mask = 0; mask < 32; ++mask) {
        seq_t dp[6], ndp[6];
        int i, j;

        /* initial column 1 */
        for (i = 0; i < 6; ++i) dp[i] = 1;

        for (j = 2; j <= 6; ++j) {
            int constrain = (mask >> (j - 2)) & 1;   /* whether j∈S */
            seq_t total = 0;
            if (!constrain) {
                /* no restriction: each new value can be any of 0–5 */
                for (i = 0; i < 6; ++i) {
                    ndp[i] = 0;
                    int k;
                    for (k = 0; k < 6; ++k) ndp[i] += dp[k];
                }
                for (i = 0; i < 6; ++i) dp[i] = ndp[i];
                continue;
            }

            /* constraint: new value ≥ previous value */
            for (i = 0; i < 6; ++i) {
                seq_t sum = 0;
                int k;
                for (k = 0; k <= i; ++k) sum += dp[k];
                ndp[i] = sum;
            }
            for (i = 0; i < 6; ++i) dp[i] = ndp[i];
        }

        /* total for this mask */
        seq_t tot = 0;
        for (i = 0; i < 6; ++i) tot += dp[i];
        f[mask] = tot;
    }
    f_computed = 1;
}

/* Number of 1‑bits in an unsigned int (population count). */
static int popcount(unsigned int x)
{
    return __builtin_popcount(x);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n≥1 expected */

    if (!f_computed) compute_f();

    seq_t sum_even = 0;  /* signs + */
    seq_t sum_odd  = 0;  /* signs - */

    int mask;
    for (mask = 0; mask < 32; ++mask) {
        seq_t term = pow_seq(f[mask], (int)n);   /* compute f[mask]^n */
        if ((popcount(mask) & 1) == 0)
            sum_even += term;
        else
            sum_odd  += term;
    }

    /* result = sum_even - sum_odd  (sum_even ≥ sum_odd). */
    seq_t neg_odd = (~sum_odd) + 1;   /* two's‑complement negation */
    return sum_even + neg_odd;
}
