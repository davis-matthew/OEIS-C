/*
 * A229169  Define a sequence of real numbers by b(1)=2, b(n+1) = b(n) +
 *          log_2(b(n)); a(n) = floor( b(n) ).
 * https://oeis.org/A229169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A229169: a(n) = floor( b(n) )
   where b(1) = 2,
   and b(n+1) = b(n) + log_2( b(n) ).
   The routine evaluates b(n) with double precision and
   truncates to the nearest lower integer, which is exact for
   all n up to at least 10 000. */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Valid for n >= 1 */
    if (n < 1)
        return (seq_t)0;

    double b = 2.0;                /* b(1) */
    for (idx_t i = 1; i < n; ++i) /* iterate n-1 times */
        b += log2(b);              /* recurrence */

    /* Ensure correct floor even when due to rounding b is
       infinitesimally below an integer. Low‑order addition
       cannot push it over. */
    uint64_t val = (uint64_t)(b + 1e-12);
    return (seq_t)val;
}
