/*
 * A180773  Number of distinct solutions of Sum_{i=1..2} (x(2i-1)*x(2i)) = 0
 *          (mod n), with x() only in 1..n-1.
 * https://oeis.org/A180773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered solutions of * x1*x2 + x3*x4 ≡ 0 (mod
 *            n) with 1 ≤ xi ≤ n-1, * counting each unordered pair of
 *            unordered pairs once
 * Range:     u8 n <= 13, u16 n <= 71, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1)=0, no x values for n=1 */
    /* frequency of unordered pairs (i,j) with i <= j, 1 <= i,j <= n-1 */
    idx_t npairs = (n - 1) * (n - 2) / 2 + (n - 1); /* not used, just reference */
    seq_t *freq = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
    if (!freq) return 0;           /* allocation failure */

    /* initialize to zero */
    for (idx_t k = 0; k < n; ++k) freq[k] = 0;

    /* count unordered pairs */
    for (idx_t i = 1; i < n; ++i) {
        for (idx_t j = i; j < n; ++j) {
            idx_t prod = (i * j) % n;
            freq[prod] += (seq_t)1;
        }
    }

    /* compute number of unordered solutions */
    seq_t result = 0;
    for (idx_t r = 0; r < n; ++r) {
        idx_t m = (n - r) % n;     /* complementary residue */
        if (r < m) {
            result += freq[r] * freq[m];
        } else if (r == m) {
            seq_t f = freq[r];
            result += f * (f + (seq_t)1) / (seq_t)2;
        }
    }

    free(freq);
    return result;
}
