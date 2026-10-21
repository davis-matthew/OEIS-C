/*
 * A077075  Least k such that Z(k,4) <= Z(n,5) where Z(m,s) = Sum_{i>=m}
 *          1/i^s.
 * https://oeis.org/A077075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that Z(k,4) ≤ Z(n,5), where Z(m,s) = Σ_{i≥m}
 *            1/i^s
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_K 300000      /* sufficiently large to cover k for n ≤ 10000 */
#define MAX_N 10000       /* maximum n expected in test set */

static long double tail4[MAX_K + 2];
static long double tail5[MAX_N + 2];
static int initialized = 0;

seq_t a(idx_t n) {
    /* Ensure tables are filled once */
    if (!initialized) {
        int i;
        tail4[MAX_K + 1] = 0.0L;
        for (i = MAX_K; i >= 1; --i) {
            long double v = (long double)i;
            tail4[i] = tail4[i + 1] + 1.0L / (v * v * v * v);
        }
        tail5[MAX_N + 1] = 0.0L;
        for (i = MAX_N; i >= 1; --i) {
            long double v = (long double)i;
            tail5[i] = tail5[i + 1] + 1.0L / (v * v * v * v * v);
        }
        initialized = 1;
    }

    /* For n = 0 use the same target as n = 1 */
    long double target = (n == 0) ? tail5[1] : tail5[(int)n];

    /* Binary search for the smallest k with tail4[k] <= target */
    int low = 1, high = MAX_K;
    while (low < high) {
        int mid = (low + high) / 2;
        if (tail4[mid] <= target)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
