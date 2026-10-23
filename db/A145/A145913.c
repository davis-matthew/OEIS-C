/*
 * A145913  a(n) = smallest integer k such that 1/log((1 + k)^(1/k)) is bigger
 *          than n.
 * https://oeis.org/A145913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer k such that k / log(k+1) > n
 * Range:     u8 n <= 45, u16 n <= 5909, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <limits.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 for this sequence */
    if (n == 0)
        return 0; /* undefined, but guard against it */

    uint64_t low = 1;
    uint64_t high = n;          /* start with a reasonable bound */

    /* Increase high until the inequality holds */
    while (true) {
        double val = (double)high / log((double)(high + 1));
        if (val > (double)n)
            break;

        /* avoid overflow – stop if we reach almost the max representable value */
        if (high > (uint64_t)ULLONG_MAX / 2) {
            high = ULLONG_MAX / 2;
            break;
        }
        high *= 2;
    }

    /* Binary search for the minimal k */
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        double val = (double)mid / log((double)(mid + 1));
        if (val > (double)n)
            high = mid;
        else
            low = mid + 1;
    }

    return (seq_t)low;
}
