/*
 * A145739  Numbers k for which the sum of all divisors of k <= sqrt(k) is a
 *          divisor of k.
 * https://oeis.org/A145739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 6618, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145739: numbers k for which the sum of all divisors of k <= sqrt(k)
   is a divisor of k.
   Computed by brute‑force search. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer square root for 64‑bit values: floor(sqrt(n)) */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = 0xFFFFFFFFULL; /* 2^32 – 1 == floor(sqrt(UINT64_MAX)) */
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) {
            return mid;
        }
        if (sq < n) {
            ans = mid;
            lo  = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined for n==0 but never called */
    }

    idx_t found = 0;   /* number of terms found so far */
    idx_t k     = 1;   /* current integer to test */

    while (found < n) {
        /* sum of divisors of k that are <= sqrt(k) */
        seq_t sum = 0;
        idx_t sqrtk = floor_isqrt_u64(k);

        for (idx_t d = 1; d <= sqrtk; ++d) {
            if (k % d == 0) {
                sum += (seq_t)d;
            }
        }

        /* check whether sum divides k */
        if (((seq_t)k) % sum == 0) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }

        ++k;
    }

    return 0; /* unreachable */
}
