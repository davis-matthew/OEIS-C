/*
 * A002637  Number of partitions of n into not more than 5 pentagonal numbers.
 * https://oeis.org/A002637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into at most 5 pentagonal
 *            numbers
 * Range:     u8 n <= 264, u16 n <= 512, u32 n <= 512, u64 n <= 512,
 *            u128 n <= 512, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    // Generate pentagonal numbers P(k) = k(3k-1)/2 up to n
    seq_t pentagons[10000];
    int num_pentagons = 0;
    seq_t n_s = (seq_t)n;
    
    for (idx_t k = 1; ; k++) {
        seq_t pent = (seq_t)k * (3 * (seq_t)k - 1) / 2;
        if (pent > n_s) break;
        pentagons[num_pentagons++] = pent;
        if (num_pentagons >= 10000) break;
    }
    
    // DP: dp[sum * 6 + count] = number of ways to partition sum into exactly count parts
    int n_int = (int)n;
    seq_t *dp = (seq_t *)calloc((n_int + 1) * 6, sizeof(seq_t));
    if (!dp) return 0;
    
    dp[0] = 1; // dp[0][0] = 1 (empty partition)
    
    // For each pentagonal number (in increasing order)
    for (int pi = 0; pi < num_pentagons; pi++) {
        int pent = (int)pentagons[pi];
        
        // Update DP: allow using this pentagonal number multiple times
        // Iterate s forward to allow reusing the same pentagonal number
        for (int s = pent; s <= n_int; s++) {
            for (int c = 1; c <= 5; c++) {
                dp[s * 6 + c] += dp[(s - pent) * 6 + (c - 1)];
            }
        }
    }
    
    // Sum over all valid partition counts (0 to 5)
    seq_t result = 0;
    for (int c = 0; c <= 5; c++) {
        result += dp[n_int * 6 + c];
    }
    
    free(dp);
    return result;
}
