/*
 * A052679  Expansion of e.g.f. (1-x^2)/(1-x^2-x^3).
 * https://oeis.org/A052679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=0; for n≥0, a(n+3) = (6+5*n+n²)*a(n+1) +
 *            (6+11*n+6*n²+n³)*a(n)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *dp = NULL;        /* memoised values */
static size_t cap = 0;          /* allocated capacity */
static idx_t last = 0;          /* highest index computed */

/* initialise the memoisation array with the first three values */
static void init(void)
{
    if (dp != NULL) return;
    cap = 3;
    dp = alloc_seq_array(cap);
    dp[0] = (seq_t)1;    /* a(0) */
    dp[1] = (seq_t)0;    /* a(1) */
    dp[2] = (seq_t)0;    /* a(2) */
    last = 2;
}

/* ensure that dp[n] has been computed for all indices up to n */
static void extend_to(idx_t n)
{
    init();
    if (n <= last) return;

    /* grow capacity to at least n+1 */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = n + 1;
        seq_t *tmp = alloc_seq_array(newcap);
        for (size_t i = 0; i <= last; ++i) tmp[i] = dp[i];
        free_seq_array(dp);
        dp = tmp;
        cap = newcap;
    }

    /* compute missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* the recurrence uses n = i-3 */
        idx_t k = i - 3;

        /* compute coefficients as unsigned 64‑bit integers */
        uint64_t c1 = 6ULL + 5ULL * k + (uint64_t)k * k;
        uint64_t c2 = 6ULL + 11ULL * k + 6ULL * (uint64_t)k * k +
                      (uint64_t)k * k * k;

        seq_t coeff1 = (seq_t)c1;
        seq_t coeff2 = (seq_t)c2;

        /* a(i) = coeff1 * a(i-2) + coeff2 * a(i-3) */
        dp[i] = coeff1 * dp[i - 2] + coeff2 * dp[i - 3];
    }
    last = n;
}

seq_t a(idx_t n)
{
    extend_to(n);
    return dp[n];
}
