/*
 * A095669  Seventh column (m=6) of (1,4)-Pascal triangle A095666.
 * https://oeis.org/A095669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+24)*binomial(n+5,5)/6
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 113, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Multiplicative binomial for k ≤ 6, safe for any seq_t */
static inline seq_t _binom_small(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    seq_t r = 1;
    for (idx_t i = 1; i <= K; ++i) {
        r *= (seq_t)(N - K + i);
        r /= (seq_t)i;   /* exact division at each step */
    }
    return r;
}

/* OEIS sequence A095669 */
seq_t a(idx_t n)
{
    /* binomial(n+5,5) */
    seq_t bin = _binom_small(n + 5, 5);

    /* (n+24)*bin/6, division is exact */
    seq_t numerator = ((seq_t)(n + 24)) * bin;
    return numerator / 6;
}
