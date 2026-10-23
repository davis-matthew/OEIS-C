/*
 * A028605  Expansion of (theta_3(z)*theta_3(10z)+theta_2(z)*theta_2(10z)).
 * https://oeis.org/A028605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = number of integer solutions (x,y) of * x^2 + 10*y^2 = n
 *            with x ≡ y (mod 2). * Each solution is counted with multiplicity
 *            4 if x≠0 and y≠0, * with multiplicity 2 if exactly one of x or y
 *            is zero, * and 1 for the trivial solution (0,0) when n = 0
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for unsigned seq_t. */
static inline seq_t
isqrt_seq(seq_t x)
{
    seq_t lo = 0, hi = x, best = 0;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0 || mid <= x / mid) {
            best = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* coefficient of z^0 is 1 */

    seq_t N = (seq_t)n;
    seq_t count = 0;

    /* maximum y satisfies 10*y^2 <= N  -->  y <= sqrt(N/10) */
    seq_t jmax = isqrt_seq(N / (seq_t)10);

    for (seq_t y = 0; y <= jmax; ++y) {
        seq_t y2 = y * y;
        seq_t rem = N - (y2 * (seq_t)10);   /* N - 10*y^2 */

        seq_t x = isqrt_seq(rem);
        if (x * x != rem)
            continue;                       /* rem is not a perfect square */

        if ((x & 1U) != (y & 1U))
            continue;                       /* parity mismatch */

        if (x == 0 || y == 0)
            count += (seq_t)2;              /* one coordinate zero, two sign choices */
        else
            count += (seq_t)4;              /* both non‑zero, four sign choices */
    }

    return count;
}
