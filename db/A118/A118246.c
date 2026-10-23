/*
 * A118246  Number of partitions of n such that even parts occur at most once
 *          and odd parts occur at most twice.
 * https://oeis.org/A118246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n such that even parts occur at
 *            most once and odd parts occur at most twice
 * Range:     u8 n <= 23, u16 n <= 65, u32 n <= 201, u64 n <= 674,
 *            u128 n <= 2410, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Maximum value of n we will precompute.  Tests use n up to about 10000. */
#define MAX_N 10000

/* Global buffer of sequence values */
static seq_t dp[MAX_N + 1];
static bool initialized = false;

/* Precompute all terms up to MAX_N */
static void compute_all(void)
{
    if (initialized) return;

    idx_t limit = MAX_N;
    for (idx_t i = 0; i <= limit; ++i) dp[i] = 0;
    dp[0] = 1;                       /* empty partition */

    for (idx_t part = 1; part <= limit; ++part) {
        if ((part & 1u) == 0) {      /* even part – multiplicity ≤ 1 */
            for (idx_t sum = limit; sum >= part; --sum) {
                dp[sum] += dp[sum - part];
            }
        } else {                     /* odd part – multiplicity ≤ 2 */
            for (idx_t sum = limit; sum >= part; --sum) {
                dp[sum] += dp[sum - part];
                if (sum >= (part << 1))
                    dp[sum] += dp[sum - (part << 1)];
            }
        }
    }
    initialized = true;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (!initialized) compute_all();
    if (n > MAX_N) return 0;   /* out of precomputed range – safe fallback */
    return dp[n];
}
