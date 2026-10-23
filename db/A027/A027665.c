/*
 * A027665  a(n)/100000 gives log_10(n) to 5 places.
 * https://oeis.org/A027665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = round(log10(n) * 100000)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 1)   /* sequence is defined for n ≥ 1 */
        return 0;

    /* Compute log10(n) using double precision and round to nearest integer. */
    double v = log10((double)n);
    uint64_t r = (uint64_t)(v * 100000.0 + 0.5);   /* round */

    return (seq_t)r;
}
