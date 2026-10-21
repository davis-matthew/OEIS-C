/*
 * A210959  Triangle read by rows in which row n lists the divisors of n
 *          starting with 1, n, the second smallest divisor of n, the second
 *          largest divisor of n, the third smallest divisor of n, the third
 *          largest divisor of n, and so on.
 * https://oeis.org/A210959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1458, u16 n <= 7069, u32 n <= 7069, u64 n <= 7069,
 *            u128 n <= 7069, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute A210959: the n-th element of the triangle in which row r lists
 * the divisors of r in the order 1, r, 2nd smallest, 2nd largest, … .
 *
 * The algorithm finds the row r containing position n by accumulating
 * divisor counts, then computes r's divisors and selects the appropriate
 * one in the zig‑zag pattern.
 */

#include <stdlib.h>

/* Count positive divisors of n (n > 0) */
static idx_t divisor_count(idx_t n)
{
    idx_t cnt = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            ++cnt;               /* i */
            if (i != n / i)
                ++cnt;           /* n / i */
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* n >= 1 by problem statement */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Find the row r that contains position n */
    idx_t sum = 0;          /* sum of divisor counts of rows < r */
    idx_t r   = 1;
    idx_t d   = 0;          /* divisor count of row r */

    while (1) {
        d = divisor_count(r);
        if (sum + d >= n)
            break;
        sum += d;
        ++r;
    }

    /* Position (0‑based) within row r */
    idx_t pos0 = n - sum - (idx_t)1;

    /* Sizes for small and large divisor arrays */
    idx_t small_size  = (d + (idx_t)1) / (idx_t)2;  /* ceil(d/2) */
    idx_t large_size  = d / (idx_t)2;               /* floor(d/2) */

    /* Allocate arrays for the divisors */
    idx_t *small = NULL;
    idx_t *large = NULL;
    if (small_size > 0) {
        small = (idx_t *)malloc(small_size * sizeof(idx_t));
        if (!small) abort();
    }
    if (large_size > 0) {
        large = (idx_t *)malloc(large_size * sizeof(idx_t));
        if (!large) { free(small); abort(); }
    }

    /* Fill small (ascending) and large (descending) arrays */
    idx_t small_cnt = 0, large_cnt = 0;
    for (idx_t i = 1; i <= r / i; ++i) {
        if (r % i == 0) {
            /* small divisor */
            if (small_cnt < small_size)
                small[small_cnt++] = i;

            /* large divisor, if distinct */
            if (i != r / i) {
                if (large_cnt < large_size)
                    large[large_cnt++] = r / i;
            }
        }
    }

    /* Choose the divisor according to zig‑zag order */
    seq_t result;
    if ((pos0 & (idx_t)1) == 0) {            /* even index → small array */
        idx_t idx = pos0 >> 1;               /* pos0 / 2 */
        result = (seq_t)(small[idx]);
    } else {                                 /* odd index → large array */
        idx_t idx = pos0 >> 1;               /* pos0 / 2 */
        result = (seq_t)(large[idx]);
    }

    /* Free temporary memory */
    free(small);
    free(large);

    return result;
}
