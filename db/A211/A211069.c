/*
 * A211069  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          determinant in [-n,n].
 * https://oeis.org/A211069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { 2×2 matrices [a b; c d] with a,b,c,d∈{1,…,n} and
 *            |ad-bc|≤n }
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* n ≥ 1 */
    idx_t N = n;
    idx_t maxp = N * N;                /* maximum product value */
    /* frequency array for products a*d */
    uint16_t *freq = (uint16_t *)malloc((maxp + 1) * sizeof(uint16_t));
    if (!freq) return 0; /* allocation failure – unlikely for n≤10000 */
    memset(freq, 0, (maxp + 1) * sizeof(uint16_t));

    /* Count products a*d */
    for (idx_t a_val = 1; a_val <= N; ++a_val) {
        idx_t prod = a_val;                   /* a*d for d=1 */
        for (idx_t d = 1; d <= N; ++d) {
            freq[prod]++;
            prod += a_val;                    /* next product a*(d+1) */
        }
    }

    /* Sliding window on sorted product values [1..maxp] */
    seq_t ans = 0;
    idx_t l = 1;
    idx_t r = (N < maxp) ? (N + 1) : maxp;   /* initial window bounds for p=1 */
    uint64_t window = 0;
    for (idx_t i = l; i <= r; ++i) {
        window += freq[i];
    }

    for (idx_t p = 1; p <= maxp; ++p) {
        ans += (seq_t)freq[p] * (seq_t)window;

        /* Prepare for next p (p+1) */
        idx_t next_l = (p + 1 > N) ? (p + 1 - N) : 1;
        idx_t next_r = (p + 1 + N <= maxp) ? (p + 1 + N) : maxp;

        if (next_l > l) {          /* left bound moved right by 1 */
            window -= freq[l];
            l = next_l;
        }
        if (next_r > r) {          /* right bound moved right by 1 */
            r = next_r;
            window += freq[r];
        }
    }

    free(freq);
    return ans;
}
