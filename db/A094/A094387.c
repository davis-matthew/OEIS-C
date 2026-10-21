/*
 * A094387  Numbers k such that gcd(k, A000120(k)) = 1.
 * https://oeis.org/A094387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n‑th positive integer k such that gcd(k, A000120(k)) = 1
   (where A000120(k) is the popcount of k).  This implementation
   simply scans all integers, checks the condition, and returns the
   requested element.  It runs in O(n) time and is well under the
   2 s limit for n ≈ 10 000.  */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm for 64‑bit unsigned integers. */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Count the number of 1‑bits in an unsigned 64‑bit integer. */
static inline uint64_t popcount_u64(uint64_t x)
{
    uint64_t c = 0;
    while (x) {
        c += x & 1ULL;
        x >>= 1;
    }
    return c;
}

/* Return the nth term of OEIS A094387.  The index is 1‑based. */
seq_t a(idx_t n)
{
    idx_t found = 0;                /* number of terms found so far */
    for (idx_t k = 1; ; ++k) {     /* scan candidate integers */
        uint64_t k64 = (uint64_t)k; /* candidate fits in 64 bits */
        uint64_t pc  = popcount_u64(k64);
        if (gcd_u64(k64, pc) == 1ULL) {
            ++found;
            if (found == n) {
                /* cast to seq_t may truncate if the value exceeds the
                   range of the target type, but for n ≈ 10 000 the
                   value fits in all supported seq_t sizes. */
                return (seq_t)k;
            }
        }
    }
}
