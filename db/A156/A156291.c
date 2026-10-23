/*
 * A156291  Triangle read by rows: T(n,m) = Product_{k=1..n} Phi(k, m+1) where
 *          Phi(k,m) is the k-th cyclotomic polynomial evaluated at m.
 * https://oeis.org/A156291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = T(r,m) where T(r,m)=∏_{k=1..r} Φ_k(m+1) and (r,m) satisfy
 *            * r(r+1)/2 ≥ n > (r-1)r/2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* integer square root, floor(sqrt(n)).  Works safely for non‑negative idx_t. */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset 1 – unused but safe */

    /* Find row r: smallest r with r(r+1)/2 ≥ n */
    idx_t r = 1;
    idx_t tri = 1;                               /* r*(r+1)/2 for current r */
    while (tri < n) {
        ++r;
        tri = r * (r + 1) / 2;
    }

    /* Column m within row r (1‑based) */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t m = n - prev_tri;                      /* 1‑based column */
    seq_t x = (seq_t)m + 1;                      /* evaluate at m+1 */

    /* Allocate array for Φ_k(x) values (1 … r). */
    size_t sz = (size_t)r + 1;                   /* phi[0] unused */
    seq_t *phi = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!phi) return 0;                          /* allocation failure: fallback */

    /* Compute Φ_1(x) = x – 1 */
    phi[1] = x - 1;
    seq_t result = phi[1];

    /* Power accumulator: x_pow = x^k */
    seq_t x_pow = x;                              /* k = 1 */

    for (idx_t k = 2; k <= r; ++k) {
        /* Update x_pow to x^k */
        x_pow = x_pow * x;

        /* Numerator: x^k – 1 */
        seq_t numer = x_pow - 1;

        /* Divide by Φ_d(x) for each proper divisor d of k */
        idx_t lim = isqrt_idx(k);
        for (idx_t d = 1; d <= lim; ++d) {
            if (k % d == 0) {
                idx_t d1 = d;
                if (d1 < k) numer = numer / phi[d1];
                idx_t d2 = k / d;
                if (d2 != d1 && d2 < k) numer = numer / phi[d2];
            }
        }

        phi[k] = numer;
        result = result * numer;
    }

    free(phi);
    return result;
}
