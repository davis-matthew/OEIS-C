/*
 * A162526  Numbers k whose largest divisor <= sqrt(k) equals 6.
 * https://oeis.org/A162526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates all multiples of 6 and returns the n‑th
   number whose largest divisor ≤ sqrt(k) equals 6.  See
   OEIS A162526. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root for uint64_t: floor(√x) */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;   /* 2^32 > sqrt(2^64-1) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Test whether k satisfies: largest divisor ≤ sqrt(k) equals 6. */
static bool is_valid(idx_t k)
{
    if (k % 6 != 0)
        return false;

    idx_t sqrt_k = isqrt_u64(k);

    /* 6 must be ≤ sqrt(k) (otherwise the largest divisor ≤ sqrt(k)
       cannot be 6). */
    if (sqrt_k < 6)
        return false;

    /* No divisor greater than 6 and ≤ sqrt(k) is allowed. */
    for (idx_t d = 7; d <= sqrt_k; ++d) {
        if (k % d == 0)
            return false;
    }
    return true;
}

/* Return the n‑th term of the sequence (a(1) = 36). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t found = 0;   /* how many valid numbers found so far */
    idx_t k     = 6;   /* current candidate (must be multiple of 6) */

    while (found < n) {
        if (is_valid(k)) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        k += 6;
    }
    /* Unreachable, but keeps the compiler happy. */
    return 0;
}
