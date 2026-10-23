/*
 * A045523  Numbers k such that k! has initial digit '4'.
 * https://oeis.org/A045523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 6416, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: incremental search using the fractional part of log10(k!).
   For each integer k, we add log10(k) to a running sum and keep only the
   fractional part.  When that fraction lies in [log10(4), log10(5))
   the current k is appended to the cache as a term of A045523.  The
   first n terms can be retrieved on demand.  The implementation
   maintains a static cache large enough for the expected range
   (≈100 000 terms for n ≤ 10 000) and works with any supported type
   for seq_t (built‑in unsigned integer, arbitrary‑precision class,
   etc.). */

#include <stdint.h>
#include <math.h>
#include <stdlib.h>

/* Auxiliary constants for the digit interval [4,5) */
static const long double LOG10_4 = 0.6020599913279624L;
static const long double LOG10_5 = 0.6989700043360189L;

/* Static cache large enough for the required range (≈110 000 terms). */
#define CACHE_CAP 200000
static seq_t cache[CACHE_CAP];
static idx_t cache_len   = 0;   /* number of terms stored */
static idx_t next_k      = 1;   /* next integer to test */
static long double frac  = 0.0L;/* fractional part of log10(k!) */

/* Main function required by the task */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* safety for out‑of‑range calls */

    while (cache_len < n)
    {
        /* Accumulate log10(next_k) and keep only its fractional part */
        frac += log10l((long double)next_k);
        frac -= floorl(frac);

        /* If (next_k)! starts with digit 4, store next_k */
        if (frac >= LOG10_4 && frac < LOG10_5)
        {
            if (cache_len == CACHE_CAP)
                abort();          /* cache overflow – should not happen */

            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
