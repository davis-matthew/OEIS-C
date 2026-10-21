/*
 * A061208  Numbers which can be expressed as sum of distinct triangular
 *          numbers (A000217).
 * https://oeis.org/A061208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer that is NOT one of
 *                {2,5,8,12,23,33}.  Equivalently a(n) = n+7 for n ≥ 27.
 *
 * This implements the definition of OEIS A061208: numbers expressible as
 * a sum of distinct triangular numbers (T_k = k(k+1)/2). 
 */
#include <stddef.h>
#include <stdbool.h>

static const idx_t missing_vals[] = {2, 5, 8, 12, 23, 33};
static const size_t missing_count = sizeof(missing_vals) / sizeof(missing_vals[0]);

/* Helper: test whether a value is one of the missing numbers */
static bool is_missing(idx_t v)
{
    for (size_t i = 0; i < missing_count; ++i) {
        if (v == missing_vals[i]) {
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Let cnt be count of numbers encountered that are NOT missing */
    idx_t cnt = 0;
    idx_t val = 1;            /* candidate integer */

    while (true) {
        if (!is_missing(val)) {
            ++cnt;
            if (cnt == n) {
                return (seq_t)val;   /* result fits in seq_t by construction */
            }
        }
        ++val;
    }
}
