/*
 * A223894  Triangular array read by rows: T(n,k) is the number of connected
 *          components with size k summed over all simple labeled graphs on n
 *          nodes; n>=1, 1<=k<=n.
 * https://oeis.org/A223894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = smallest integer with r(r+1)/2 ≥ n, k =
 *            n – r(r–1)/2, and T(r,k) = C(r,k) * c(k) * 2^{(r−k)(r−k−1)/2}.
 *            Here c(k) is the number of connected labeled graphs on k nodes
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 77, u128 n <= 152,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b); */

/* Integer square root for idx_t (works up to 64-bit values). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n + 1;
    while (high - low > 1) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Purely integer power of two: 2^e.  e is small (≤ ~5000). */
static inline seq_t pow2(idx_t e)
{
    seq_t res = 1;
    for (idx_t i = 0; i < e; ++i)
        res *= 2;
    return res;
}

/* Storage for numbers of connected labeled graphs: c[1] … c[maxN]. */
static seq_t *c = NULL;
static size_t c_cap = 0;
static idx_t maxN = 0;

/* Ensure the array has room for at least 'need' elements. */
static void ensure_seq_cap(size_t need, size_t *cap, seq_t **ptr)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    if (new_cap < need)
        new_cap = need;
    void *tmp = realloc(*ptr, new_cap * sizeof(seq_t));
    *ptr = (seq_t *)tmp;
    *cap = new_cap;
}

/* Compute c[1..m] using the recurrence. */
static void compute_c(idx_t m)
{
    if (m <= maxN)
        return;

    ensure_seq_cap(m + 1, &c_cap, &c);
    /* Compute new values from maxN+1 up to m. */
    for (idx_t n = maxN + 1; n <= m; ++n) {
        seq_t total = pow2(n * (n - 1) / 2);  /* 2^{C(n,2)} */
        seq_t sum = 0;
        seq_t bin = 1;  /* binomial(n-1,0) */
        for (idx_t k = 1; k < n; ++k) {
            seq_t term = bin;
            term *= c[k];
            idx_t t = n - k;
            term *= pow2(t * (t - 1) / 2);
            sum += term;
            /* Update binomial(n-1, k-1) to binomial(n-1, k). */
            bin *= (seq_t)(n - k);
            bin /= (seq_t)k;
        }
        c[n] = total - sum;
    }
    maxN = m;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* not defined, but safe guard */

    /* Find row r: smallest with r(r+1)/2 >= n. */
    idx_t s = isqrt_u64(1 + 8 * n);
    idx_t r = (s - 1) / 2;
    if (r * (r + 1) / 2 < n)
        ++r;

    idx_t k = n - r * (r - 1) / 2;  /* column, 1‑based */

    compute_c(r);

    seq_t bin = binomial(r, k);
    idx_t rem = r - k;
    seq_t pow_term = pow2(rem * (rem - 1) / 2);

    seq_t result = bin;
    result *= c[k];
    result *= pow_term;

    return result;
}
