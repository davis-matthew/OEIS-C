/*
 * A220945  Number of partitions of n into non-consecutive distinct squares.
 * https://oeis.org/A220945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 422, u16 n <= 4458, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A220945: number of partitions of n into distinct
 * non‑consecutive squares. This implementation uses dynamic programming
 * over the squares k².  Two arrays are maintained for each
 * intermediate step:  dp0[j] counts ways to reach sum j without using
 * the current square; dp1[j] counts ways using the current square.
 * After processing all squares ≤ n,  a(n) = dp0[n] + dp1[n]. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Array size: 0 .. n inclusive.  For the tested limits n ≤ 10 000 */
    size_t len = (size_t)n + 1;

    /* Allocate DP arrays, zero‑initialized by the helper. */
    seq_t *dp0 = alloc_seq_array(len);
    seq_t *dp1 = alloc_seq_array(len);
    if (!dp0 || !dp1) {
        return (seq_t)0;
    }

    dp0[0] = (seq_t)1;   /* empty partition */
    /* dp1[0] is already zero */

    /* Process squares k² for k = 1, 2, … while k² ≤ n. */
    for (idx_t k = 1;; ++k) {
        seq_t sq = (seq_t)k * (seq_t)k;
        if (sq > (seq_t)n) {
            break;
        }

        /* Allocate arrays for the next state. */
        seq_t *new0 = alloc_seq_array(len);
        seq_t *new1 = alloc_seq_array(len);
        if (!new0 || !new1) {
            free_seq_array(dp0);
            free_seq_array(dp1);
            return (seq_t)0;
        }

        /* sq fits into size_t because n ≤ 10000 on the intended inputs. */
        size_t sq_idx = (size_t)sq;

        /* Update DP tables. */
        for (size_t j = 0; j < len; ++j) {
            new0[j] = dp0[j] + dp1[j];        /* do not use the current square */
            if (j >= sq_idx) {
                new1[j] = dp0[j - sq_idx];    /* use the current square */
            } else {
                new1[j] = (seq_t)0;
            }
        }

        /* Free previous arrays and advance. */
        free_seq_array(dp0);
        free_seq_array(dp1);
        dp0 = new0;
        dp1 = new1;
    }

    seq_t result = dp0[(size_t)n] + dp1[(size_t)n];

    free_seq_array(dp0);
    free_seq_array(dp1);

    return result;
}
