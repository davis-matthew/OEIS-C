/*
 * A078607  Least positive integer x such that 2*x^n > (x+1)^n.
 * https://oeis.org/A078607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(1/(2^(1/n)-1)) (n>1, a(0)=1, a(1)=2)
 * Range:     u8 n <= 177, u16 n <= 9697, u32 n <= 9697, u64 n <= 9697,
 *            u128 n <= 9697, big any n
 * Helpers:   _cmp_check
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/_cmp_check.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Initial approximation using n / log_e(2) ≈ 1.442695... * n */
    long double approx_ld = ((long double)n) / logl(2.0L);
    uint64_t approx   = (uint64_t)(approx_ld + 0.5L);  /* nearest integer */
    if (approx < 1) approx = 1;

    /* Ensure high bound satisfies the inequality */
    uint64_t low  = 1;
    uint64_t high = approx + 2;
    while (!_cmp_check(n, high))
        high = high * 2 + 2;  /* increase until condition holds */

    /* Binary search for the smallest x such that 2*x^n > (x+1)^n */
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        if (_cmp_check(n, mid))
            high = mid;
        else
            low = mid;
    }
    return (seq_t)high;
}
