/*
 * A162020  Positive k such that there is at least one partition into parts
 *          {14, 52, 78, 133, 248} (terms of A113907).
 * https://oeis.org/A162020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) is the n-th positive integer representable as a sum of any
 *            number of the elements of * the set {14,52,78,133,248}. For n >
 *            163, a(n) = n + 164
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* static initialization */
    static bool initialized = false;
    static seq_t values[328];      /* enough to hold all values <= 327 */
    static idx_t count = 0;        /* number of representable values <= 327 */

    if (!initialized) {
        const int MAX_REACHED = 327;
        const int COIN_COUNT  = 5;
        static const seq_t COINS[5] = {
            (seq_t)14,
            (seq_t)52,
            (seq_t)78,
            (seq_t)133,
            (seq_t)248
        };

        bool reach[MAX_REACHED + 1];
        int i, c;

        for (i = 0; i <= MAX_REACHED; ++i) reach[i] = false;
        reach[0] = true;

        /* unbounded knapsack DP */
        for (c = 0; c < COIN_COUNT; ++c) {
            int coin = (int)COINS[c];
            for (i = coin; i <= MAX_REACHED; ++i) {
                reach[i] = reach[i] || reach[i - coin];
            }
        }

        /* collect representable numbers in ascending order */
        for (i = 1; i <= MAX_REACHED; ++i) {
            if (reach[i]) {
                values[count++] = (seq_t)i;
            }
        }

        initialized = true;
    }

    if (n <= count) {
        return values[n - 1];
    } else {
        /* For n > count (i.e., n > 163), all integers > 327 are in the set.
           Thus the (n - count)-th missing number is 327, so a(n) = n + 164. */
        return (seq_t)(n + 164);
    }
}
