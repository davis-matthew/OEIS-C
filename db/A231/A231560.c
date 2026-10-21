/*
 * A231560  Floor(sum_{i=2..n} 1/(i*log(i))).
 * https://oeis.org/A231560
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ_{i=2}^n 1/(i*log(i)) )
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /*  OEIS offset 2 : terms for n < 2 are not defined. */
    if (n < 2) return (seq_t)0ULL;

    double sum = 0.0;
    for (idx_t i = 2; i <= n; ++i) {
        double di = (double)i;   /* cast to double for the log */
        sum += 1.0 / (di * log(di));
    }

    /*  Use a small epsilon to avoid floating‑point corner cases */
    double tmp = floor(sum + 1e-12);

    /*  Convert the floating result to an unsigned 64‑bit integer
        then to the target seq_t type.  This avoids a direct cast
        from double to seq_t, which may not be defined for
        arbitrary‑precision integers. */
    idx_t ival = (idx_t)tmp;
    return (seq_t)ival;
}
