/*
 * A212109  Number of (w,x,y,z) with all terms in {1,...,n} and w*x-y*z<=n.
 * https://oeis.org/A212109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Formula: a(n) = number of 4‑tuples (w,x,y,z), 1≤w,x,y,z≤n, with
 *               w*x - y*z ≤ n.
 *
 *  Algorithm: 1) Count how many ordered pairs (i,j) have each product
 *   prod = i*j (freq[prod]).
 *  2) For each product value p the number of (y,z) with y*z ≥ p-n is
 *   n² - pre[p-n-1], where pre[s] is the number of pairs with product ≤ s.
 *  3) Compute the sum Σ freq[p] * (n² - pre[p-n-1]) while iterating
 *   over p from 1 to n².  A small circular buffer holds the latest
 *   prefix sums so pre[p-n-1] can be retrieved in O(1) without storing
 *   the whole prefix array.
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* maximum product */
    const idx_t N = n * n;
    /* frequency array for products 1..N, 0 unused */
    uint16_t *freq = (uint16_t *)malloc((N + 1) * sizeof(uint16_t));
    if (!freq)
        return 0;            /* allocation failure – fallback */
    memset(freq, 0, (N + 1) * sizeof(uint16_t));

    /* Count product frequencies */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = 1; j <= n; ++j) {
            ++freq[i * j];
        }
    }

    /* Circular buffer for latest prefix sums.  Size n+1 is enough */
    const size_t bufSize = (size_t)n + 1;
    uint64_t *buf = (uint64_t *)malloc(bufSize * sizeof(uint64_t));
    if (!buf) {
        free(freq);
        return 0; /* fallback on allocation failure */
    }
    memset(buf, 0, bufSize * sizeof(uint64_t));

    seq_t result = 0;
    const seq_t totalPairs = (seq_t)n * (seq_t)n;   /* n²  */
    uint64_t prefixSum = 0;                          /* runs prefix sum */

    for (idx_t p = 1; p <= N; ++p) {
        const uint16_t fp = freq[p];

        /* pre[p-n-1] is needed only if p > n */
        uint64_t pre_val = 0;
        if (p > n) {
            idx_t idx = p - n - 1;
            /* idx is at most n-1, buffer is size n+1 */
            pre_val = buf[idx % bufSize];
        }

        /* Number of Q with Q ≥ p-n */
        seq_t avail = totalPairs - (seq_t)pre_val;
        result += (seq_t)fp * avail;

        /* Update prefix sum and circular buffer */
        prefixSum += fp;
        buf[p % bufSize] = prefixSum;
    }

    free(freq);
    free(buf);
    return result;
}
