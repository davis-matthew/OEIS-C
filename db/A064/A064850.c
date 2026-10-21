/*
 * A064850  Period of continued fraction for sqrt(5)*n.
 * https://oeis.org/A064850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = period length of continued fraction for sqrt(5)*n
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the simply continued‑fraction period of sqrt(5*n^2),
   using the standard "a0, m, d, a" recurrence.  The loop terminates when the
   period repeats, i.e. when a_k == 2*a0. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for 64‑bit values using binary search.
   Returns floor(sqrt(x)).  Uses 128‑bit multiplies to avoid overflow. */
static seq_t isqrt_u64(uint64_t x)
{
    uint64_t low = 0, high = x, ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (seq_t)ans;
}

/* Returns the period length of the continued fraction of sqrt(5)*n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, but safe */

    /* D = 5*n^2  (fits in 64 bits for the intended range) */
    uint64_t nn = (uint64_t)n * (uint64_t)n;
    uint64_t D  = 5ULL * nn;

    /* a0 = floor(sqrt(D)) */
    uint64_t a0 = (uint64_t)isqrt_u64(D);
    if (a0 * a0 == D) return 0; /* perfect square (n=0 only) */

    uint64_t m = 0, d = 1;
    uint64_t a_curr = a0, two_a0 = 2 * a0;
    uint64_t period = 0;

    do {
        /* m = d*a_curr - m */
        m = d * a_curr - m;
        /* d = (D - m^2) / d */
        uint64_t m_sq = m * m;
        d = (D - m_sq) / d;
        /* a_curr = (a0 + m) / d */
        a_curr = (a0 + m) / d;
        period++;
    } while (a_curr != two_a0);

    return (seq_t)period;
}
