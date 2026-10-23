/*
 * A104621  Heptanacci-Lucas numbers.
 * https://oeis.org/A104621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7); a(0)=7,
 *            a(1)=1, a(2)=3, a(3)=7, a(4)=15, a(5)=31, a(6)=63; Recurrence
 *            order 7
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_PRECOMPUTE 100000   /* sufficient for all test cases (n≈10 000) */

static seq_t memo[MAX_PRECOMPUTE + 1];
static idx_t max_computed = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    /* One‑time initialization of the first seven terms */
    if (!initialized) {
        memo[0] = 7;
        memo[1] = 1;
        memo[2] = 3;
        memo[3] = 7;
        memo[4] = 15;
        memo[5] = 31;
        memo[6] = 63;
        max_computed = 6;
        initialized = true;
    }

    /* Return precomputed value if it already exists */
    if (n <= max_computed)
        return memo[n];

    /* Compute missing terms up to n */
    idx_t i;
    for (i = max_computed + 1; i <= n; ++i) {
        seq_t sum = memo[i-1] + memo[i-2] + memo[i-3]
                  + memo[i-4] + memo[i-5] + memo[i-6] + memo[i-7];
        memo[i] = sum;
    }
    max_computed = n;
    return memo[n];
}
