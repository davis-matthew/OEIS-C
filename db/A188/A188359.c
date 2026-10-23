/*
 * A188359  Number of arrangements of 4 numbers x(i) in -n..n with the sum of
 *          x(i)*x(i+1) equal to zero.
 * https://oeis.org/A188359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to x1*x2 + x2*x3 + x3*x4 = 0 * with
 *            each xi ∈ {‑n, …, n}. The computation uses an exact * counting
 *            argument that separates the cases where x2 or x3 is zero * from
 *            the cases where both are non‑zero. For the latter, the *
 *            Diophantine equation p*x1 + q*x4 = –p*q (p = x2, q...
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline int64_t floordiv(int64_t a, int64_t b)
{
    assert(b != 0);
    int64_t q = a / b;
    int64_t r = a % b;
    if ((r != 0) && ((r > 0) != (b > 0)))
        --q;
    return q;
}

static inline int64_t ceildiv(int64_t a, int64_t b)
{
    assert(b != 0);
    int64_t q = a / b;
    int64_t r = a % b;
    if ((r != 0) && ((r > 0) == (b > 0)))
        ++q;
    return q;
}

seq_t a(idx_t n)
{
    /* Case 1: at least one of x2 or x3 is zero.
     * Number of such solutions is (2n+1)*(6n+1). */
    idx_t m = (idx_t)(2 * n + 1);            /* size of range S */
    seq_t result = (seq_t)m * (seq_t)(6 * n + 1);

    /* Case 2: both x2 and x3 are non‑zero. */
    for (idx_t a_abs = 1; a_abs <= n; ++a_abs) {
        for (idx_t b_abs = 1; b_abs <= n; ++b_abs) {
            idx_t d = gcd_idx(a_abs, b_abs);
            /* reduced coprime components */
            idx_t alpha = b_abs / d;          /* positive */
            idx_t beta  = a_abs / d;          /* positive */

            /* bounds from q - n <= alpha*t <= q + n, with q = b_abs */
            int64_t L = (int64_t)b_abs - (int64_t)n;
            int64_t R = (int64_t)b_abs + (int64_t)n;

            /* alpha > 0, so direct bounds on t */
            int64_t t_low = ceildiv(L, (int64_t)alpha);
            int64_t t_high = floordiv(R, (int64_t)alpha);

            /* bounds from |beta*t| <= n, i.e. |t| <= floor(n/|beta|) */
            int64_t tbmax = (int64_t)n / (int64_t)beta;
            if (t_low < -tbmax) t_low = -tbmax;
            if (t_high > tbmax)  t_high = tbmax;

            if (t_low <= t_high) {
                /* each absolute pair (abs(p),abs(q)) corresponds to
                 * four sign combinations of (p,q). */
                seq_t add = (seq_t)(t_high - t_low + 1);
                add *= (seq_t)4;
                result += add;
            }
        }
    }
    return result;
}
