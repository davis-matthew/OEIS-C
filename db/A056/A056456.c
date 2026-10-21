/*
 * A056456  Number of palindromes of length n using exactly five different
 *          symbols.
 * https://oeis.org/A056456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5! * Stirling2( floor((n+1)/2), 5 )
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 26, u64 n <= 54, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute S(n,5) for n up to 5000 and cache the results.
 * S[0] .. S[5000] store Stirling numbers of the second kind S(n,5).
 * cur[0..5] holds the current row of Stirling numbers S(n,k). */
static seq_t S[5001] = {0};
static seq_t cur[6] = {1, 0, 0, 0, 0, 0};
static idx_t max_precomputed = 0;

static void ensure_precomputed(idx_t m)
{
    /* Extend the precomputed table up to m (if needed). */
    if (m <= max_precomputed)
        return;
    /* cur[0] == 0 for n > 0; cur[0] after first iteration will become 0. */
    for (idx_t n = max_precomputed + 1; n <= m; ++n) {
        /* Update cur from the previous row (n-1). */
        for (int k = 5; k >= 1; --k) {
            /* cur[k] = k * cur[k] + cur[k-1] */
            cur[k] = cur[k] * (seq_t)k + cur[k - 1];
        }
        cur[0] = 0;
        S[n] = cur[5];
    }
    max_precomputed = m;
}

seq_t a(idx_t n)
{
    /* m = floor((n + 1)/2) */
    idx_t m = (n + 1) / 2;
    ensure_precomputed(m);
    /* Multiply by 5! = 120 */
    return S[m] * (seq_t)120;
}
