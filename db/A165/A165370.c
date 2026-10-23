/*
 * A165370  Smallest number whose sum of cubes of digits is n.
 * https://oeis.org/A165370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   The smallest nondecreasing digit number whose sum of cubes of
 *            digits equals n. This implementation uses DP to find the minimal
 *            number of digits, then reconstructs the lexicographically
 *            smallest sorted-digit number
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 4278, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

static const int cube[10] = {0,1,8,27,64,125,216,343,512,729};

seq_t a(idx_t n) {
    /* Special case */
    if (n == 0) return (seq_t)0;

    /* Static DP tables */
    static int *dpCount = NULL;
    static idx_t dpSize = 0;
    static idx_t dpMax  = 0;

    /* Allocate / extend DP array as needed */
    if (dpCount == NULL) {
        dpSize = 1;
        dpCount = (int*)malloc(sizeof(int));
    }
    if (n >= dpSize) {
        idx_t newSize = n + 1;
        int *newArr = (int*)realloc(dpCount, newSize * sizeof(int));
        if (!newArr) return (seq_t)0;  /* allocation failure */
        dpCount = newArr;
        dpSize  = newSize;
    }

    /* Initialize DP up to n */
    if (dpMax == 0) dpCount[0] = 0;  /* dpCount[0] = 0 */
    for (idx_t s = dpMax + 1; s <= n; ++s) {
        int best = INT_MAX;
        for (int d = 1; d <= 9; ++d) {
            int c = cube[d];
            if (s >= c) {
                int val = dpCount[s - c] + 1;
                if (val < best) best = val;
            }
        }
        dpCount[s] = best;
    }
    dpMax = n;

    /* Reconstruct lexicographically smallest sorted digits */
    int remaining = (int)n;
    int digitsNeeded = dpCount[remaining];
    int *dig = (int*)malloc(digitsNeeded * sizeof(int));
    int pos = 0;
    while (remaining > 0) {
        for (int d = 1; d <= 9; ++d) {
            int c = cube[d];
            if (remaining >= c && dpCount[remaining - c] + 1 == dpCount[remaining]) {
                dig[pos++] = d;
                remaining -= c;
                break;
            }
        }
    }

    /* Build the numeric result */
    seq_t res = (seq_t)0;
    const seq_t ten = (seq_t)10;
    for (int i = 0; i < digitsNeeded; ++i) {
        res = res * ten + (seq_t)dig[i];
    }

    free(dig);
    return res;
}
