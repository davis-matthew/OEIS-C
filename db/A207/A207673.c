/*
 * A207673  n+[nr/s]+[nt/s], where []=floor, r=5, s=(1+sqrt(5))/2, t=1/s.
 * https://oeis.org/A207673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*5/s) + floor(n*t/s) where s = (1+sqrt(5))/2
 *            and t = 1/s
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const long double ALPHA = 3.0901699437494741515951463L; /* 5/s = 5/φ   */
static const long double BETA  = 0.3819660112501051515840949L; /* 1/φ²       */

seq_t a(idx_t n)
{
    /* Use long double to compute the two floors accurately.
       For n up to the size of 64‑bit indices the error is far below 0.5,
       so truncating to unsigned long long yields the correct floor.   */
    unsigned long long f1 = (unsigned long long)((long double)n * ALPHA);
    unsigned long long f2 = (unsigned long long)((long double)n * BETA);
    return (seq_t)n + (seq_t)f1 + (seq_t)f2;
}
