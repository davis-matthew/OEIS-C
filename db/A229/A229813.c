/*
 * A229813  G.f. C(x) satisfies: C(x) = x + 3*A(x)*B(x), where A(x) = x +
 *          B(x)*C(x) and B(x) = x + 2*A(x)*C(x).
 * https://oeis.org/A229813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A229813. The coefficients are obtained from the system of
 *            generating–function equations C = x + 3*A*B, A = x + B*C, B = x
 *            + 2*A*C. From these relations the coefficient recurrences p(1) =
 *            q(1) = a(1) = 1, p(n) = Σ_{i=1}^{n-1} q(i)·a(n-i), q(n) =
 *            2·Σ_{i=1}^{n-1} p(i)·a(n-i), a(n) = 3·Σ_{i=1}^{n-1}...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* size_t */

#define MAX_N 10000

/* Pre‑allocate tables for the three generating‑function series.  */
static seq_t p_cache[MAX_N + 1] = {0};
static seq_t q_cache[MAX_N + 1] = {0};
static seq_t c_cache[MAX_N + 1] = {0};
static idx_t  cache_last = 0;      /* last index that has been computed */

/* Compute terms up to n (inclusive).  The function assumes 1 ≤ n ≤ MAX_N. */
static void compute_up_to(idx_t n)
{
    if (n < 1) return;

    if (cache_last == 0) {            /* First time: initialise the base. */
        p_cache[1] = 1;
        q_cache[1] = 1;
        c_cache[1] = 1;
        cache_last = 1;
    }

    for (idx_t m = cache_last + 1; m <= n; ++m) {
        seq_t pm = 0, qm = 0, cm = 0;

        /* p(m) = Σ_{i=1}^{m-1} q(i)·c(m-i) */
        for (idx_t i = 1; i < m; ++i)
            pm += q_cache[i] * c_cache[m - i];

        /* q(m) = 2·Σ_{i=1}^{m-1} p(i)·c(m-i) */
        for (idx_t i = 1; i < m; ++i)
            qm += (seq_t)2 * p_cache[i] * c_cache[m - i];

        /* a(m) = 3·Σ_{i=1}^{m-1} p(i)·q(m-i) */
        for (idx_t i = 1; i < m; ++i)
            cm += (seq_t)3 * p_cache[i] * q_cache[m - i];

        p_cache[m] = pm;
        q_cache[m] = qm;
        c_cache[m] = cm;
    }

    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n < 1)         /* undefined for n < 1 – return 0 */
        return 0;

    if (n > (idx_t)MAX_N)
        return 0;     /* out of range – caller should guard against it */

    compute_up_to(n);
    return c_cache[n];
}
