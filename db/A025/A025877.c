/*
 * A025877  Expansion of 1/((1-x^5)*(1-x^6)*(1-x^8)).
 * https://oeis.org/A025877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-5)+a(n-6)+a(n-8), with a(0)=1 and a(n)=0 for negative n
 * Range:     u8 n <= 339, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* allocate DP array of size n+1 */
    size_t N = (size_t)n;
    seq_t *dp = alloc_seq_array(N + 1);
    if (dp == NULL) return 0; /* allocation failure, unlikely */

    dp[0] = 1;                 /* one way to form 0 */

    static const idx_t parts[3] = {5, 6, 8};

    for (size_t k = 0; k < 3; ++k) {
        size_t p = (size_t)parts[k];
        for (size_t i = p; i <= N; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[N];
    free_seq(dp);
    return result;
}
