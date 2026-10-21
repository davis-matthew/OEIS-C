/*
 * A097851  G.f.:
 *          (1+x^8+x^9+x^10+x^18)/((1-x^2)*(1-x^3)*(1-x^4)*(1-x^5)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A097851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n)+p(n-8)+p(n-9)+p(n-10)+p(n-18), where p(m) is the
 *            number of partitions of m with parts 2,3,4,5,6,7 (p(m)=0 for
 *            m<0)
 * Range:     u8 n <= 25, u16 n <= 90, u32 n <= 872, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;          /* partition counts for parts 2..7 */
static idx_t dp_size = 0;         /* array length (indices 0..dp_size-1) */

/* Compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* Ensure the dp array is large enough for n and all required shifts (up to n-18). */
    idx_t need = n + 18;                     /* largest index we will query */
    if (need >= dp_size) {
        idx_t new_size = need + 1;
        size_t alloc_bytes = (size_t)new_size * sizeof(seq_t);
        seq_t *newdp = (seq_t *)realloc(dp, alloc_bytes);
        if (!newdp) return 0;                /* allocation failure: return 0 as fallback */

        /* Initialise the new array: p(0)=1, all others 0. */
        newdp[0] = 1;
        for (idx_t i = 1; i < new_size; ++i) newdp[i] = 0;

        /* Dynamic programming to compute p(i) for 0 <= i <= need.
           For each allowed part size k (2..7), add ways to form i by using k. */
        for (idx_t k = 2; k <= 7; ++k) {
            for (idx_t i = k; i < new_size; ++i) {
                newdp[i] += newdp[i - k];
            }
        }

        dp = newdp;
        dp_size = new_size;
    }

    /* Sum the needed shifted values. */
    seq_t ans = dp[n];
    if (n >= 8)  ans += dp[n - 8];
    if (n >= 9)  ans += dp[n - 9];
    if (n >= 10) ans += dp[n - 10];
    if (n >= 18) ans += dp[n - 18];
    return ans;
}
