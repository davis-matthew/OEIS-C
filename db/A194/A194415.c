/*
 * A194415  Numbers m such that Sum_{k=1..m} (<1/3 + k*r> - <k*r>) < 0, where
 *          r=sqrt(3) and < > denotes fractional part.
 * https://oeis.org/A194415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A194415:
 * a(n) = n-th integer m such that
 * sum_{k=1..m} ( {k*sqrt(3)+1/3} - {k*sqrt(3)} ) < 0
 * Let f_k = {k*sqrt(3)}. Then
 *   {k*sqrt(3)+1/3} - {k*sqrt(3)} = 1/3  if f_k < 2/3,
 *                                   = -2/3 if f_k >= 2/3.
 * Let C(m) be the number of k ≤ m with f_k < 2/3.
 * Then S(m) = C(m)·(1/3) + (m-C(m))·(-2/3) = C(m) - 2m/3.
 * S(m) < 0 ⇔ 3·C(m) < 2·m.
 * We compute C(m) by checking whether
 * √(3m²) < ⌊√(3m²)⌋ + 2/3, which yields an integer inequality
 *     9·(3m²-⌊√(3m²)⌋²) < 12·⌊√(3m²)⌋ + 4 .
 * This test uses only integer arithmetic.
 *
 * The implementation iterates m = 1,2,.. until the n‑th occurrence
 * of 3·C(m) < 2·m is found.  For n up to about 10⁴ this is
 * well under the time limit. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) for 64‑bit values using binary search.
 * Works for all 0 ≤ n < 2^64. */
static inline uint64_t floor_sqrt_u64(uint64_t n)
{
    uint64_t low = 0;
    uint64_t high = (uint64_t)1 << 32;      /* sqrt(max_uint64) < 2^32 */
    while (low < high) {
        uint64_t mid = low + (high - low + 1) / 2;
        if (mid > 0 && mid > n / mid)      /* mid*mid > n → reduce high */
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* the sequence is defined for n ≥ 1 */
        return 0;

    /* We need up to n terms.  m will not grow very large for n ≤ 10000
     * (roughly 2.5·n).  All intermediate calculations fit in 64‑bit. */
    uint64_t m = 0;            /* current index m */
    uint64_t count_small = 0;  /* C(m) */
    uint64_t found = 0;        /* number of terms found so far */

    while (found < n) {
        ++m;
        /* Compute t = 3·m².  For the expected ranges this fits into
         * 64 bits.  Use 128‑bit intermediate to avoid overflow. */
        __uint128_t t128 = (__uint128_t)3 * m * m;
        uint64_t t = (uint64_t)t128;          /* safe because t < 2^64 */

        uint64_t a_sqrt = floor_sqrt_u64(t);  /* ⌊√t⌋ = ⌊m·√3⌋ */
        uint64_t delta  = t - a_sqrt * a_sqrt;/* 0 ≤ delta < 2·a_sqrt+1 */

        /* Test whether fractional part of m·√3 is < 2/3:
         * 9·delta < 12·a_sqrt + 4   (derived above) */
        uint64_t left  = 9u * delta;
        uint64_t right = 12u * a_sqrt + 4u;
        if (left < right)
            ++count_small;

        /* Check the condition 3·C(m) < 2·m */
        if (3 * count_small < 2 * m) {
            ++found;
            if (found == n)
                return (seq_t)m;            /* n‑th term found */
        }
    }

    /* Should never reach here */
    return (seq_t)m;
}
