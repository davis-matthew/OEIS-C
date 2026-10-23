/*
 * A107324  Floor(A063655(n)/2).
 * https://oeis.org/A107324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(average of the middle divisor(s) of n) (OEIS
 *            A107324)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Compute the function a(n) as described above. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n = 0, just return 0 */

    /* Find integer square root of n. */
    idx_t sqrt_n = (idx_t)floor(sqrt((long double)n));

    /* Allocate arrays to hold small and large divisors */
    idx_t max_cnt = sqrt_n + 1;
    seq_t *small = (seq_t*)malloc(sizeof(seq_t) * max_cnt);
    seq_t *large = (seq_t*)malloc(sizeof(seq_t) * max_cnt);
    if (!small || !large) return 0;   /* allocation failed */

    idx_t small_cnt = 0, large_cnt = 0;

    /* Enumerate divisors up to sqrt(n). */
    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            small[small_cnt++] = (seq_t)i;
            idx_t j = n / i;
            if (j != i) large[large_cnt++] = (seq_t)j;
        }
    }

    /* Total divisor count */
    idx_t total = small_cnt + large_cnt;
    seq_t result;

    if (total & 1) {  /* odd number of divisors */
        idx_t middle = (total + 1) / 2;   /* 1‑based index */
        if (middle <= small_cnt) {
            result = small[middle - 1];
        } else {
            /* large array is stored in decreasing order, so reverse index */
            idx_t idx = large_cnt - (middle - small_cnt);
            result = large[idx];
        }
    } else {          /* even number of divisors */
        idx_t left_idx  = total / 2;       /* 1‑based */
        idx_t right_idx = left_idx + 1;

        seq_t left_val, right_val;

        /* helper to fetch value by 1‑based index */
        if (left_idx <= small_cnt) left_val = small[left_idx - 1];
        else left_val = large[large_cnt - (left_idx - small_cnt)];

        if (right_idx <= small_cnt) right_val = small[right_idx - 1];
        else right_val = large[large_cnt - (right_idx - small_cnt)];

        /* floor((left_val + right_val)/2) without overflow */
        seq_t lhalf = left_val / 2;
        seq_t rhalf = right_val / 2;
        seq_t rem_sum = (left_val % 2) + (right_val % 2);
        result = lhalf + rhalf + rem_sum / 2;
    }

    free(small);
    free(large);
    return result;
}
