/*
 * A212136  Number of (w,x,y,z) with all terms in {1,...,n} and w*x+y*z>n^2.
 * https://oeis.org/A212136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of (w,x,y,z) ∈ {1,…,n}⁴ with w*x + y*z > n². The
 *            algorithm counts all product frequencies for pairs (w,x) and
 *            (y,z), then uses prefix sums to compute the number of quadruples
 *            with w*x + y*z ≤ n². Finally a(n) = n⁴ – that count
 * Range:     u8 n <= 6, u16 n <= 29, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* n may fit in idx_t; n*n fits in idx_t (n ≤ 10⁴). */
    idx_t N = n * n;           /* maximum product value */
    size_t sz = (size_t)N + 1; /* include index 0 */

    /* Frequency of each product w*x: freq[p] counts pairs (w,x) with w*x = p. */
    uint16_t *freq = (uint16_t *)calloc(sz, sizeof(uint16_t));
    if (!freq)
        return 0; /* failed allocation */

    for (idx_t w = 1; w <= n; ++w)
        for (idx_t x = 1; x <= n; ++x)
            ++freq[w * x];

    /* Prefix sums of frequencies: pref[p] = Σ_{k=1}^{p} freq[k] */
    uint32_t *pref = (uint32_t *)calloc(sz, sizeof(uint32_t));
    if (!pref) {
        free(freq);
        return 0;
    }

    seq_t running = 0;
    for (idx_t i = 1; i <= N; ++i) {
        running += (seq_t)freq[i];
        pref[i] = (uint32_t)running;
    }

    /* Count of quadruples with w*x + y*z ≤ N */
    seq_t count_le = 0;
    for (idx_t p = 1; p <= N; ++p) {
        uint16_t c_u = freq[p];
        if (!c_u)
            continue;
        seq_t c = (seq_t)c_u;
        idx_t lim = N - p;
        uint32_t pref_cnt = pref[lim];
        count_le += c * (seq_t)pref_cnt;
    }

    free(freq);
    free(pref);

    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;     /* n² */
    seq_t n4    = n_sq * n_sq;       /* n⁴ */

    return n4 - count_le;
}
