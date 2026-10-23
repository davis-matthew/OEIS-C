/*
 * A134994  a(n) = floor(3n/2log(n)).
 * https://oeis.org/A134994
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3*n/(2*log(n)))
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h> /* for uint64_t */

#define LOG_SCALE 1000000000000ULL /* 10^12 scaling factor for ln(n) */

seq_t a(idx_t n)
{
    /* The OEIS offset is 2; values for n < 2 are arbitrary */
    if (n < 2)
        return (seq_t)0;

    /* Compute ln(n) with double precision and scale it to an
       integer to avoid using floating arithmetic in the final
       division.  The cast to uint64_t performs a truncation
       (floor). */
    double ln_d   = log((double)n);            /* natural log, double */
    uint64_t ln_s = (uint64_t)(ln_d * (double)LOG_SCALE);

    /* Use 128‑bit arithmetic to prevent overflow when computing
       the numerator (3*n*LOG_SCALE).  All other values fit
       comfortably in 128 bits for n up to at least 10⁹. */
    __int128 num = (__int128)3 * n * LOG_SCALE; /* 3*n*10^12   */
    __int128 den = (__int128)2 * ln_s;          /* 2*ln(n)*10^12 */

    /* Integer division gives the floor of the exact fraction. */
    seq_t result = (seq_t)(num / den);
    return result;
}
