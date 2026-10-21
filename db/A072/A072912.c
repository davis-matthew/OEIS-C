/*
 * A072912  Number of Fibonacci numbers F(k) <= 10^n which end in 0.
 * https://oeis.org/A072912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072912: Number of Fibonacci numbers F(k) ≤ 10^n that end in 0.
 * The count is 1 (for F(0)=0) plus the number of indices k≥15 with
 * k≡0 (mod 15) such that F(k) ≤ 10^n.
 * Let Kmax be the largest k with F(k) ≤ 10^n.  Then
 *   a(n) = ⌊Kmax/15⌋ + 1.
 * The largest such k can be found using the digit‑count formula
 *   digits(F(k)) = ⌊k·log10(φ) – log10(√5)⌋ + 1,
 * where φ is the golden ratio.  A binary search on k yields Kmax
 * in O(log n) time.  No large integer arithmetic is needed – the
 * digit formula is exact for all k≥1.
 */

#include <math.h>   /* floorl */

/* Constants in long double (high precision). */
static const long double LOG10PHI  = 0.208987640249978733L; /* log10(phi)   */
static const long double LOG10SQRT5 = 0.349485002168009400L; /* log10(sqrt5) */

static inline idx_t fib_digits(idx_t k)
{
    if (k == 0) return 1;                    /* F(0) = 0 has 1 digit */
    long double val = k * LOG10PHI - LOG10SQRT5;
    /* add tiny epsilon to guard against an under‑rounding that might
       push val just below an integer value.  The epsilon is far
       smaller than the precision of long double for all k considered. */
    val += 1e-12L;
    return (idx_t)floorl(val) + 1;
}

seq_t a(idx_t n)
{
    /* Upper bound for the index: k ≈ n / log10(phi) ≈ 4.78 * n.
       Adding a small safety margin. */
    idx_t low  = 0;
    idx_t high = (idx_t)(n * 5ULL + 20ULL);   /* safe for all n ≤ 10000 */

    while (low < high) {
        idx_t mid = (low + high + 1ULL) >> 1ULL;
        if (fib_digits(mid) <= n)
            low = mid;
        else
            high = mid - 1ULL;
    }

    /* low is now the largest k with F(k) ≤ 10^n */
    idx_t kmax = low;
    seq_t count = (seq_t)(kmax / 15ULL + 1ULL); /* include F(0) */
    return count;
}
