/*
 * A064368  Number of 2 X 2 symmetric singular matrices with entries from
 *          {0,...,n}.
 * https://oeis.org/A064368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 1 + 2 * Σ_{d=1}^{⌊√n⌋} φ(d) * ⌊ n / d² ⌋
 * Range:     u8 n <= 53, u16 n <= 8575, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The totient helper is supplied by the build system in utils/phi.h   */
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Handle n == 0 (off‑by‑offset correction)          */
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    /* Compute floor(√n) by simple integer loop (n ≤ ≈10⁵ in tests)  */
    idx_t sq = 0;
    while ((sq + 1) * (sq + 1) <= n)
        ++sq;

    seq_t sum = 0;
    for (idx_t d = 1; d <= sq; ++d)
    {
        idx_t cnt = n / (d * d);             /* ⌊ n / d² ⌋   */
        seq_t phi_d = phi(d);                /* φ(d)         */
        sum += phi_d * (seq_t)cnt;
    }

    seq_t result = (seq_t)n + (seq_t)1 + (seq_t)2 * sum;
    return result;
}
