/*
 * A070752  Numbers k such that sin(k) > 0.
 * https://oeis.org/A070752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th positive integer k such that sin(k) > 0.
   Equivalently, floor(k / π) must be even.  The implementation iterates
   over the Beatty blocks defined by floor(m·π) and floor((m+1)·π). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0) return 0;

    /* Scale π to 18 decimal places (floor of π·10¹⁸). */
    const unsigned long long SCALE   = 1000000000000000000ULL;    /* 1e18   */
    const unsigned long long PI_NUM  = 3141592653589793238ULL;    /* floor(π·SCALE) */
    const unsigned long long PI_INT  = PI_NUM / SCALE;            /* integer part (3) */
    const unsigned long long PI_FRAC = PI_NUM % SCALE;            /* fractional part */

    /* Current block index m, floor(m·π) and remainder modulo SCALE */
    unsigned long long m          = 0;          /* block number */
    unsigned long long floor_m    = 0;          /* floor(m·π) */
    unsigned long long rem        = 0;          /* m·π (mod SCALE) */

    idx_t produced = 0;   /* number of terms generated so far */

    while (1) {
        /* Start of current block (ceil(m·π)) */
        unsigned long long start = floor_m + 1;   /* for m==0 gives 1 */

        /* Compute floor((m+1)·π) using incremental arithmetic */
        unsigned long long new_floor = floor_m + PI_INT;
        unsigned long long new_rem   = rem + PI_FRAC;
        if (new_rem >= SCALE) {
            new_floor += 1;
            new_rem -= SCALE;
        }
        unsigned long long end   = new_floor;            /* floor((m+1)·π) */
        unsigned long long len   = (end >= start) ? (end - start + 1) : 0;

        if ((m & 1ULL) == 0) {          /* even m → include block */
            if (produced + (idx_t)len >= n) {
                idx_t offset = n - produced - 1;       /* zero‑based offset */
                return (seq_t)(start + (unsigned long long)offset);
            }
            produced += (idx_t)len;
        }

        /* Advance to next block */
        floor_m = new_floor;
        rem     = new_rem;
        ++m;
    }
}
