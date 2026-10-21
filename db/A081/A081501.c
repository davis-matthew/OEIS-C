/*
 * A081501  a(n) = row sum of triangle whose n-th row begins with n and
 *          contains n-1 smallest numbers coprime to n and greater than n.
 * https://oeis.org/A081501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = n + Σ_{k > n, gcd(k,n)=1} k (take the smallest n‑1 such
 *            k's)
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Euclidean algorithm for uint64_t (used to test coprimality without casting to seq_t) */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* For safety handle the unlikely case n == 0 (offset starts at 1). */
    if (n == 0) return 0;

    seq_t sum = (seq_t)n;          /* first element of the row */
    idx_t  count = 0;              /* how many coprimes found after n  */
    uint64_t candidate = (uint64_t)n + 1;  /* next integer to test */

    /* Find the next (n‑1) coprime numbers greater than n */
    while (count < n - 1)
    {
        if (gcd_uint64((uint64_t)n, candidate) == 1ULL)
        {
            sum += (seq_t)candidate;
            ++count;
        }
        ++candidate;
    }

    return sum;
}
