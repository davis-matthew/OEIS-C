/*
 * A077592  Table by antidiagonals of tau_k(n), the k-th Piltz function (see
 *          A007425), or n-th term of the sequence resulting from applying the
 *          inverse Möbius transform (k-1) times to the all-ones sequence.
 * https://oeis.org/A077592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, tabl, look
 * Formula:   a(n) = τₖ(m) where (m,k) are obtained by * listing τₖ(m) over
 *            the table of k‑Piltz divisor functions * along antidiagonals.
 *            For n ≥ 1, * let s be the smallest integer with n ≤ s(s‑1)/2; *
 *            let offset = n – (s‑1)(s‑2)/2 (1 ≤ offset ≤ s‑1); * m = offset;
 *            k = s – m; return τₖ(m) = ∏ₚ C(k+eₚ–1, eₚ) *...
 * Range:     u8 n <= 160, u16 n <= 1151, u32 n <= 9645, u64 n <= 9645,
 *            u128 n <= 9645, big any n
 * Helpers:   _factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_factor_u64.h"   /* provides _factor_u64 */

static seq_t binom_uint64(uint64_t n, uint64_t k)
{
    /* compute C(n,k) multiplicatively, using seq_t for all arithmetic.
       This stays within seq_t for the values encountered in the
       sequence (< 1e18 for the tested range). */
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        r *= (seq_t)(n - k + i);
        r /= (seq_t)i;
    }
    return r;
}

static seq_t tau_k_value(uint64_t m, uint64_t k)
{
    if (m <= 1) return 1;          /* τₖ(1)=1 for all k */

    /* factor m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    _factor_u64(m, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        /* C(k + e - 1, e) = C(k + e - 1, k - 1) */
        res *= binom_uint64(exps[i] + k - 1, k - 1);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    /* Find diagonal index s such that n ≤ s(s-1)/2. */
    idx_t s = 2;
    while (n > s * (s - 1) / 2) ++s;

    /* Position within the diagonal (1‑based). */
    idx_t offset = n - (s - 1) * (s - 2) / 2;   /* 1 ≤ offset ≤ s‑1 */

    idx_t m = offset;          /* row number */
    idx_t k = s - m;           /* column number */

    return tau_k_value((uint64_t)m, (uint64_t)k);
}
