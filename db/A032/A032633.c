/*
 * A032633  a(n) = floor(n^3 / Pi).
 * https://oeis.org/A032633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^3 / pi)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by binary search using tight rational bounds for pi:
   103993/33102 < pi < 104348/33215   (convergent bounds).   */
#include <stdint.h>

static const unsigned long long UB_NUM = 104348ULL; /* upper bound numerator  */
static const unsigned long long UB_DEN = 33215ULL;  /* upper bound denominator */
static const unsigned long long LB_NUM = 103993ULL; /* lower bound numerator  */
static const unsigned long long LB_DEN = 33102ULL;  /* lower bound denominator */

seq_t a(idx_t n)
{
    /* Compute n^3 as a seq_t (fits 64‑bit for n ≤ 10 000). */
    seq_t nb  = (seq_t)n;
    seq_t n3  = nb * nb * nb;            /* n^3 */
    unsigned long long n3ull = (unsigned long long)n3;

    unsigned long long low  = 0;
    unsigned long long high = n3ull / 3 + 1;   /* rough upper bound for n^3 / pi */
    unsigned long long result = 0;

    while (low <= high) {
        unsigned long long mid = (low + high) >> 1;

        /* Condition 1: UB * mid <= n^3   (UB = UB_NUM/UB_DEN) */
        unsigned long long lhs = UB_NUM * mid;          /* <= 3.2×10^16 */
        unsigned long long rhs = UB_DEN * n3ull;        /* <= 3.3×10^16 */

        if (lhs <= rhs) {
            /* Candidate: check upper (m+1) using lower bound */
            unsigned long long lhs2 = LB_NUM * (mid + 1);
            unsigned long long rhs2 = LB_DEN * n3ull;

            if (lhs2 > rhs2) {
                result = mid;          /* fits the inequality:
                                           UB*mid <= n^3 < LB*(mid+1)   */
                break;
            }
            low = mid + 1;
        } else {
            if (mid == 0) break;       /* avoid underflow */
            high = mid - 1;
        }
    }

    return (seq_t)result;
}
