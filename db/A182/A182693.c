/*
 * A182693  a(n) = the smallest 3-digit number with exactly n divisors, a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A182693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = smallest 3‑digit integer with exactly n divisors, 0 if none exists */
#include <stdbool.h>

static idx_t divisor_count(idx_t n) {
    idx_t count = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (i * i == n) {
                ++count;
            } else {
                count += 2;
            }
        }
    }
    return count;
}

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t ans[33];          /* indices 0..32; ans[1..32] hold results */
    static idx_t divCount[900];    /* divisor counts for numbers 100..999 */

    if (!initialized) {
        /* compute divisor counts for 100 … 999 */
        for (idx_t i = 0; i < 900; ++i) {
            divCount[i] = divisor_count(100 + i);
        }

        /* initialize answer array to 0 */
        for (idx_t i = 0; i <= 32; ++i) {
            ans[i] = 0;
        }

        /* find the smallest number for each divisor count */
        for (idx_t i = 0; i < 900; ++i) {
            seq_t k = 100 + i;
            idx_t d = divCount[i];
            if (d <= 32 && ans[d] == 0) {
                ans[d] = k;
            }
        }
        initialized = true;
    }

    if (n < 1 || n > 32) {
        return 0;
    }
    return ans[n];
}
