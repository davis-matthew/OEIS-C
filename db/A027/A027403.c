/*
 * A027403  Number of labeled servers of dimension 16.
 * https://oeis.org/A027403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A027403: Number of labeled servers of dimension 16
 *
 * This sequence is defined by the exponential generating function
 *
 *     exp( sum_{i=1}^{16} ((1+x)^i - 1)/i )
 *
 * Equivalently, with connected–component e.g.f coefficients
 *
 *     c_k = k! * Sum_{i=k}^{16} C(i,k)/i
 *          = Sum_{j=0}^{16-k} (j+k-1)! / j!
 *
 * The ordinary e.g.f recurrence for the set construction is
 *
 *     a_0 = 1
 *     a_n = Σ_{k=1}^{min(16,n)} binom(n-1,k-1) * c_k * a_{n-k}   (n≥1)
 *
 * where binom(m,r) is the ordinary binomial coefficient.
 * The implementation below uses only integer arithmetic and is
 * valid for any seq_t (8‑, 16‑, 32‑, 64‑bit or arbitrary‑precision).
 * It runs in O(16·n) time, easily within the limits for n≈10000.
 */

#include <stdlib.h>

/* Binomial coefficient C(m,r) for small r (r≤16) using exact division. */
static seq_t small_binom(idx_t m, idx_t r, const seq_t *fact)
{
    if (r == 0) return 1;
    seq_t res = 1;
    for (idx_t i = 0; i < r; ++i)
        res *= (seq_t)(m - i);
    res /= fact[r];
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Pre‑compute factorials up to 16. */
    seq_t fact[17];
    fact[0] = 1;
    for (idx_t i = 1; i <= 16; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Pre‑compute the c_k values (k=1..16). */
    seq_t c[17];   /* c[0] unused */
    for (idx_t k = 1; k <= 16; ++k) {
        seq_t ck = 0;
        for (idx_t j = 0; j <= 16 - k; ++j)
            ck = ck + fact[j + k - 1] / fact[j];
        c[k] = ck;
    }

    /* Allocate array for a[0..n]. */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) return 0;   /* allocation failure; caller can handle */

    arr[0] = 1;   /* base case */

    /* Dynamic programming using the recurrence. */
    for (idx_t cur = 1; cur <= n; ++cur) {
        seq_t sum = 0;
        idx_t maxk = cur < 16 ? cur : 16;
        for (idx_t k = 1; k <= maxk; ++k) {
            seq_t bin = small_binom(cur - 1, k - 1, fact);
            sum = sum + bin * c[k] * arr[cur - k];
        }
        arr[cur] = sum;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
