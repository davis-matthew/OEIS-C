/*
 * A025057  Position of 3n+2 in A025051 (numbers of form j*k + k*i + i*j,
 *          without repetitions, where 1 <= i <= j <= k).
 * https://oeis.org/A025057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of 3*n+2 in the increasing sequence of values
 *            i*j + i*k + j*k with 1 <= i <= j <= k
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t target = (seq_t)3 * n + 2;             /* Value whose position we need */
    size_t t = (size_t)target;                  /* Array size is manageable (≤~30000) */

    /* Mark which integers ≤ target appear in the set. */
    bool *present = (bool *)malloc((t + 1) * sizeof(bool));
    if (!present) return (seq_t)0;              /* Memory failure – unlikely */
    for (size_t i = 0; i <= t; ++i) present[i] = false;

    for (idx_t i = 1; ; ++i) {
        if ((seq_t)i * i * 3 > target) break;   /* i^2*3 is minimal value for this i */
        for (idx_t j = i; ; ++j) {
            seq_t min_val = (seq_t)2 * i * j + (seq_t)j * j;  /* k = j */
            if (min_val > target) break;
            seq_t denom = (seq_t)i + j;
            seq_t max_k = (target - (seq_t)i * j) / denom;
            if (max_k < j) continue;
            for (idx_t k = j; k <= max_k; ++k) {
                seq_t val = (seq_t)i * j + (seq_t)k * denom;
                present[(size_t)val] = true;
            }
        }
    }

    size_t count = 0;
    for (size_t v = 1; v <= t; ++v)
        if (present[v]) ++count;

    free(present);
    return (seq_t)count;
}
