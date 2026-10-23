/*
 * A193243  Number of representations of n as sum of three positive
 *          biquadrates, i.e., fourth powers.
 * https://oeis.org/A193243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 9894, u32 n <= 9894, u64 n <= 9894,
 *            u128 n <= 9894, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of unordered representations of n as a sum
 * of three positive fourth powers (biquadrates).
 * a(n) = |{ (i,j,k) : i,j,k ≥ 1, i ≤ j ≤ k, i^4 + j^4 + k^4 = n }|
 * Example: a(18) = 1 because 18 = 1^4 + 1^4 + 2^4.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;                 /* minimal sum is 1^4+1^4+1^4 = 3 */

    seq_t n_seq = (seq_t)n;
    seq_t count = 0;

    /* find maximum integer whose fourth power is ≤ n */
    idx_t max = 1;
    for (;; ++max) {
        seq_t v = (seq_t)max * max * max * max;
        if (v > n_seq) break;
    }
    --max; /* last value that satisfies v <= n_seq */

    /* pre‑compute fourth powers up to max */
    seq_t *pows = (seq_t*)malloc((max + 1) * sizeof(seq_t));
    if (!pows) return 0; /* out of memory: return 0 as a safe fallback */
    pows[0] = 0;
    for (idx_t i = 1; i <= max; ++i) {
        pows[i] = (seq_t)i * i * i * i;
    }

    /* count unordered triples */
    for (idx_t a = 1; a <= max; ++a) {
        seq_t a4 = pows[a];
        if (a4 > n_seq) break;
        for (idx_t b = a; b <= max; ++b) {
            seq_t ab4 = a4 + pows[b];
            if (ab4 > n_seq) break;
            for (idx_t c = b; c <= max; ++c) {
                seq_t sum = ab4 + pows[c];
                if (sum == n_seq) ++count;
                if (sum > n_seq) break;
            }
        }
    }

    free(pows);
    return count;
}
