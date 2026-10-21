/*
 * A211058  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          nonnegative determinant.
 * https://oeis.org/A211058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( n⁴ + Σ_{1≤a,b≤n} min(⌊n/(a/gcd(a,b))⌋ ,
 *            ⌊n/(b/gcd(a,b))⌋ ) ) / 2
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Euclidean algorithm for idx_t (unsigned 64‑bit). */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute OEIS sequence A211058. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Precompute ⌊n/d⌋ for all d.  */
    idx_t *floor_n_div = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    for (idx_t d = 1; d <= n; ++d)
        floor_n_div[d] = n / d;

    __int128 equal_cnt = 0;

    for (idx_t a_val = 1; a_val <= n; ++a_val) {
        for (idx_t b_val = 1; b_val <= n; ++b_val) {
            idx_t g = gcd_idx(a_val, b_val);
            idx_t a_prime = a_val / g;
            idx_t b_prime = b_val / g;

            /* tmax = min(⌊n/a'⌋ , ⌊n/b'⌋) */
            idx_t tmax = floor_n_div[a_prime];
            idx_t other = floor_n_div[b_prime];
            if (other < tmax)
                tmax = other;

            equal_cnt += tmax;
        }
    }

    free(floor_n_div);

    __int128 total = (__int128)n * n * n * n;  /* n⁴ */
    seq_t result = (seq_t)((total + equal_cnt) / 2);
    return result;
}
