/*
 * A034097  Fractional part of square root of a(n) starts with digit 1.
 * https://oeis.org/A034097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 6528, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A034097:  Numbers n such that m² + m/5 < n ≤ m² + 2m/5 for some
 *  positive integer m.  This function returns the nth term of the
 *  increasing sequence (offset 1), computed by iterating over m and
 *  generating the integers that lie in each interval.
 *
 *  Each interval for fixed m is:
 *       L = m² + m/5          (exclusive)
 *       R = m² + 2m/5         (inclusive)
 *
 *  The integers in the interval are
 *       start = floor(L)+1 = m² + floor(m/5) + 1
 *       end   = floor(R)   = m² + floor(2m/5)
 *
 *  The count of terms contributed by m is  floor(2m/5) - floor(m/5).
 *  We accumulate these counts until the requested index n is reached
 *  and then return the appropriate term.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Ensure n is at least 1; behavior for n < 1 is undefined. */
    idx_t pos = 0;          /* number of terms already passed */
    idx_t m   = 1;          /* current m */

    for (;;) {
        /* Compute floor divisions using integer division of idx_t. */
        idx_t qm5  = m / 5;
        idx_t q2m5 = (2*m) / 5;

        /* Count of numbers in this interval. */
        idx_t len = q2m5 - qm5;

        if (pos + len >= n) {
            /* The desired term falls within this interval. */
            seq_t msq        = (seq_t)m * (seq_t)m;        /* m² as seq_t */
            seq_t start = msq + (seq_t)qm5 + (seq_t)1; /* start of interval */
            return start + (seq_t)(n - pos - 1);
        }

        /* Skip this interval and proceed to the next m. */
        pos += len;
        ++m;
    }
}
