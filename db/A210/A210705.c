/*
 * A210705  Number of partitions of n into parts congruent to +-3, +-7, +-15,
 *          +-21, +-33, +-35, +-39, +-49, +-51, +-57 (mod 126).
 * https://oeis.org/A210705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to ±3, ±7,
 *            ±15, ±21, ±33, ±35, ±39, ±49, ±51, ±57 (mod 126)
 * Range:     u8 n <= 107, u16 n <= 275, u32 n <= 795, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Empty partition contributes 1 to a(0). */
    if (n == 0) {
        return (seq_t)1;
    }

    const size_t N = (size_t)n;

    /* dp[i] will store the number of partitions of i. */
    seq_t *dp = alloc_seq_array(N + 1);
    if (!dp) {
        return (seq_t)0;        /* allocation failure: return 0 */
    }

    for (size_t i = 0; i <= N; ++i) {
        dp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;

    /* Residues that are allowed, together with their negatives
       modulo 126.  They form the complete set of allowed part sizes.
       For each residue r we generate all part sizes r + 126*m <= N. */
    static const int residues[20] = {
        3, 7, 15, 21, 33, 35, 39, 49, 51, 57,
        69, 75, 77, 87, 91, 93, 105, 111, 119, 123
    };

    for (size_t ri = 0; ri < 20; ++ri) {
        const size_t residue = (size_t)residues[ri];
        for (size_t coin = residue; coin <= N; coin += 126) {
            for (size_t i = coin; i <= N; ++i) {
                dp[i] += dp[i - coin];
            }
        }
    }

    const seq_t result = dp[N];
    free_seq_array(dp);
    return result;
}
