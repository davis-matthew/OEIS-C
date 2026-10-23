/*
 * A138494  a and b are integers > 0 satisfying a^2 + b^2 = c^2. Sequence
 *          gives the number of choices for a and b between successive values
 *          of c. (Integer solutions for c (Pythagorean triples) are not
 *          included.)
 * https://oeis.org/A138494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |{ (a,b) ∈ ℕ² : n² < a²+b² < (n+1)² }|, n ≥ 1
 * Range:     u8 n <= 160, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A(n) is the number of positive integer pairs whose Euclidean
   distance from the origin lies strictly between the integers n
   and n+1.  The implementation counts, for each a ≤ n, the
   admissible values of b using integer square‑roots. */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 ≤ x ≤ 2⁶⁴−1.  */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;          /* skip the zero case which would loop forever */
            continue;
        }
        if (mid <= x / mid) {   /* mid*mid ≤ x without overflow */
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; for safety */

    idx_t n2     = n * n;
    idx_t np1    = n + 1;
    idx_t np1_2  = np1 * np1;
    seq_t total  = 0;

    for (idx_t a = 1; a <= n; ++a) {
        idx_t a2 = a * a;

        /* lower bound on b: b² > n² – a²  →  b > sqrt(n² – a²) */
        idx_t low  = isqrt_u64(n2 - a2) + 1;

        /* upper bound on b: b² < (n+1)² – a²  →  b < sqrt((n+1)² – a² – 1) */
        idx_t high = isqrt_u64(np1_2 - a2 - 1);

        if (high >= low) {
            total += (seq_t)(high - low + 1);
        }
    }

    return total;
}
