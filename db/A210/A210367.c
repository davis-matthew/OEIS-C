/*
 * A210367  Number of 2 X 2 matrices with all elements in {0,1,...,n} and
 *          determinant >= 2n.
 * https://oeis.org/A210367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2×2 matrices with entries in {0,…,n} whose
 *            determinant ad – bc is at least 2n
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 305, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The distribution of products ad (0 ≤ a,d ≤ n).  */
    size_t limit = (size_t)n * (size_t)n;          /* maximum product */
    size_t size  = limit + 1;                      /* array length */
    /*  We store counts in 16‑bit to keep the array small.
        Each count ≤ n+1 ≤ 10001 for n ≤ 10000, so 16‑bit is safe. */
    uint16_t *freq = (uint16_t *)calloc(size, sizeof(uint16_t));
    if (!freq) return 0;   /* allocation failure is not expected */

    /* Count all pairs (a,d).  */
    for (idx_t a = 0; a <= n; ++a) {
        for (idx_t d = 0; d <= n; ++d) {
            size_t prod = (size_t)a * (size_t)d;   /* 0 ≤ prod ≤ n²  */
            freq[prod]++;                           /* fits into uint16_t */
        }
    }

    /* Now compute the answer using a single pass over the product array.
       Let H(x) = number of pairs (b,c) with bc ≤ x.  The required sum
       is  Σ_{t ≥ 2n} freq[t] * H(t–2n).  We maintain H(t–2n) (called
       older_prefix) incrementally.  */
    seq_t ans = 0;
    seq_t older_prefix = 0;
    size_t offset = 2 * (size_t)n;   /* 2n */

    for (size_t t = 0; t <= limit; ++t) {
        if (t >= offset) {
            older_prefix += (seq_t)freq[t - offset];
            ans += (seq_t)freq[t] * older_prefix;
        }
    }

    free(freq);
    return ans;
}
