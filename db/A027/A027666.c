/*
 * A027666  a(n)/10000000 gives log_10(n) to 7 places.
 * https://oeis.org/A027666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = round(log10(n) * 10000000)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Compute log10(n) as a long double for high precision. */
    long double ln = log10((long double)n);
    /* Scale by 10^7 and round to nearest integer. */
    long double scaled = ln * 10000000.0L;
    long long rounded = llroundl(scaled);
    /* Convert to the sequence type. */
    return (seq_t)rounded;
}
