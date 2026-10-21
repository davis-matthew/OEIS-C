/*
 * A089807  Expansion of Jacobi theta function (3theta_3(q^9)-theta_3(q))/2.
 * https://oeis.org/A089807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; a(n)=0 if n is not a perfect square; a(n)=-1 if n is a
 *            perfect square and sqrt(n) not divisible by 3; a(n)= 2 if n is a
 *            perfect square and sqrt(n) divisible by 3. This follows from the
 *            multiplicative definition: a(n)=-b(n) where b(3^e)=-(1+(-1)^e)
 *            and b(p^e)=(1+(-1)^e)/2 for p≠3, and b is...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root for unsigned 64‑bit values (floor). */
static uint64_t floor_sqrt_u64(uint64_t x)
{
    /* Binary search between 1 and 0xffffffff (sqrt(2^64-1)). */
    uint64_t low = 1;
    uint64_t high = 0xffffffffULL;
    uint64_t ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t div = x / mid;
        if (mid == div) {          /* mid^2 == x */
            return mid;
        }
        if (mid < div) {           /* mid^2 < x */
            ans = mid;
            low = mid + 1;
        } else {                   /* mid^2 > x */
            high = mid - 1;
        }
    }
    return ans;
}

extern seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    uint64_t sq = floor_sqrt_u64((uint64_t)n);
    if (sq * sq != n) {
        return (seq_t)0;          /* not a perfect square */
    }

    /* perfect square */
    if (sq % 3 == 0) {
        return (seq_t)2;          /* sqrt(n) divisible by 3 */
    } else {
        return (seq_t)-1;         /* sqrt(n) not divisible by 3 */
    }
}
