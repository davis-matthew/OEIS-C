/*
 * A194964  a(n) = 1 + floor(n/sqrt(5)).
 * https://oeis.org/A194964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n / sqrt(5))
 * Range:     u8 n <= 570, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For the given limits (n up to about 10 000) 64‑bit arithmetic is
       sufficient for all intermediate values.  */
    unsigned long long nt = (unsigned long long)n;

    /* target = 5 * n²  (the number we will take the integer square root of) */
    unsigned long long target = nt * nt * 5ULL;

    /* Binary search for floor(sqrt(target)).  The result is < 3*n, so we
       use 3*n as an upper bound (safe for the required n range).  */
    unsigned long long low  = 0;
    unsigned long long high = nt * 3ULL;      /* 3*n */

    while (low < high) {
        unsigned long long mid = (low + high + 1) >> 1; /* upper mid to avoid infinite loop */
        if (mid * mid <= target)
            low = mid;
        else
            high = mid - 1;
    }

    unsigned long long sqrt_floor = low;
    unsigned long long div        = sqrt_floor / 5ULL;

    return (seq_t)(div + 1ULL);
}
