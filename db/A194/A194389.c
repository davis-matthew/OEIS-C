/*
 * A194389  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) > 0, where
 *          r=sqrt(11) and < > denotes fractional part.
 * https://oeis.org/A194389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A194389:
   a(n) = the n-th integer m such that
   Sum_{k=1}^{m} ( frac(1/2 + k*sqrt(11)) - frac(k*sqrt(11)) ) > 0
   where frac(x) denotes the fractional part of x.
   The condition can be tested using only integer operations. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n < 2^64.
   Binary search using 64‑bit arithmetic. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0;
    uint64_t high = ((uint64_t)1 << 32); /* > sqrt(UINT64_MAX) */
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        if (mid > n / mid)      /* mid*mid would overflow or be too large */
            high = mid;
        else
            low = mid;
    }
    return low;
}

/* Returns the n‑th term (1‑based index) of the sequence. */
seq_t a(idx_t n)
{
    uint64_t found = 0;     /* number of terms found so far */
    uint64_t k = 0;         /* current m being examined */
    int64_t diff = 0;       /* cumulative sum of (+1 for frac <= 1/2, -1 otherwise) */

    while (found < n) {
        ++k;

        /* Compute floor(k*sqrt(11)) and floor(2*k*sqrt(11)).
           Those are floor_sqrt(11*k^2) and floor_sqrt(44*k^2). */
        uint64_t kk = k * k;
        uint64_t a_k = isqrt_u64(11 * kk);
        uint64_t b_k = isqrt_u64(44 * kk);

        /* Condition: b_k <= 2*a_k  <=>  fractional part of k*sqrt(11) <= 0.5 */
        if (b_k <= (a_k << 1))
            diff += 1;   /* plus sign */
        else
            diff -= 1;   /* minus sign */

        if (diff > 0) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
    }

    /* The loop always returns; this point is unreachable. */
    return (seq_t)k;
}
