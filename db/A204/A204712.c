/*
 * A204712  Number of (n+1) X 8 0..1 arrays with the permanents of all 2 X 2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 8*a(n-1) + 50*a(n-2)
 *            - 389*a(n-3) - 790*a(n-4) + 6534*a(n-5) + 3836*a(n-6) -
 *            45232*a(n-7) + 9360*a(n-8) + 108928*a(n-9) - 82304*a(n-10) with
 *            initial values a(1)=385, a(2)=3473, a(3)=28161, a(4)=239425,
 *            a(5)=1992321, a(6)=16748161, a(7)=140090241, a(8)=1...
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation memoises already computed values. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ----------  Helper definitions (must exist in the build)  ---------- */
/* seq_t and idx_t are described in the build headers; do not define them. */

/* ----------  Implementation ---------- */

#define MAX_N 20000                /* Enough for requested n up to ~10‑000. */

static const seq_t init_values[11] = {
    (seq_t)0,                      /* a(0) unused */
    (seq_t)385,                    /* a(1) */
    (seq_t)3473,                   /* a(2) */
    (seq_t)28161,                  /* a(3) */
    (seq_t)239425,                 /* a(4) */
    (seq_t)1992321,                /* a(5) */
    (seq_t)16748161,               /* a(6) */
    (seq_t)140090241,              /* a(7) */
    (seq_t)1174759297,             /* a(8) */
    (seq_t)9838208513,             /* a(9) */
    (seq_t)82449830017             /* a(10) */
};

static seq_t cache[MAX_N + 1] = { (seq_t)0 };
static uint8_t computed[MAX_N + 1] = { 0 };

/* Constant multipliers, cast to seq_t to avoid unsigned truncation. */
static const seq_t C8      = (seq_t)8;
static const seq_t C50     = (seq_t)50;
static const seq_t C389    = (seq_t)389;
static const seq_t C790    = (seq_t)790;
static const seq_t C6534   = (seq_t)6534;
static const seq_t C3836   = (seq_t)3836;
static const seq_t C45232  = (seq_t)45232;
static const seq_t C9360   = (seq_t)9360;
static const seq_t C108928 = (seq_t)108928;
static const seq_t C82304  = (seq_t)82304;

/* ------------------------------------------------------------ */
/* Returns a(n) for n ≥ 1.                                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) {
        return (seq_t)0;   /* undefined for n < 1 */
    }
    if (n > (idx_t)MAX_N) {
        return (seq_t)0;   /* out of range for this implementation */
    }

    /* Compute terms up to n on demand, memoising each. */
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        if (computed[(size_t)i]) {
            continue;   /* already computed */
        }
        if (i <= (idx_t)10) {
            cache[(size_t)i] = init_values[(size_t)i];
        } else {
            seq_t pos = C8      * cache[(size_t)(i - 1)] +
                        C50     * cache[(size_t)(i - 2)] +
                        C6534   * cache[(size_t)(i - 5)] +
                        C3836   * cache[(size_t)(i - 6)] +
                        C9360   * cache[(size_t)(i - 8)] +
                        C108928 * cache[(size_t)(i - 9)];
            seq_t neg = C389    * cache[(size_t)(i - 3)] +
                        C790    * cache[(size_t)(i - 4)] +
                        C45232  * cache[(size_t)(i - 7)] +
                        C82304  * cache[(size_t)(i - 10)];
            cache[(size_t)i] = pos - neg;   /* guaranteed nonnegative for valid i */
        }
        computed[(size_t)i] = 1;
    }

    return cache[(size_t)n];
}
