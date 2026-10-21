/*
 * A080500  a(n) = (n-1)(n-4)(n-9)...(n-k^2) where k^2 < n <= (k+1)^2.
 * https://oeis.org/A080500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{⌊√(n‑1)⌋} (n - k²)
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* to satisfy compiler when using bool */

static idx_t isqrt_u64(idx_t n)
{
    /* simple integer square‑root, safe for n up to at least 2¹⁶ */
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) {
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* return 1 for n = 1 (empty product) */
    if (n <= 1) {
        return (seq_t)1;
    }

    /* floor sqrt(n–1) */
    idx_t kmax = isqrt_u64(n - 1);

    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t term = (seq_t)(n - k * k);
        prod *= term;
    }
    return prod;
}
