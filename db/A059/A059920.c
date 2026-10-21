/*
 * A059920  If m/n = q + r/n (r < n, n,m >=1), then array a(m,n) = qr (meaning
 *          q followed by r). Read by antidiagonals.
 * https://oeis.org/A059920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, tabl, nonn
 * Formula:   a(n) = decimal concatenation of floor(m/n) and (m mod n), where
 *            the pair (m,n) is retrieved by reading the integer grid a(m,n)
 *            along anti-diagonals (offset 1)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) */
static idx_t isqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find diagonal sum S such that S*(S-1)/2 >= n */
    idx_t t = 8 * n + 1;
    idx_t sqrt_t = isqrt(t);
    idx_t S = (1 + sqrt_t) >> 1;
    if ((S * (S - 1)) / 2 < n)
        ++S;

    /* Number of terms before this diagonal */
    idx_t prev = ((S - 1) * (S - 2)) / 2;
    idx_t offset = n - prev;          /* 1-based position within diagonal */

    /* Corresponding (m,n) pair */
    idx_t m_val = offset;             /* m in a(m,n) */
    idx_t d_val = S - offset;         /* n in a(m,n) */

    /* Compute floor(m/n) and remainder */
    idx_t q_idx = m_val / d_val;
    idx_t r_idx = m_val % d_val;

    seq_t q = (seq_t)q_idx;
    seq_t r = (seq_t)r_idx;

    /* Determine 10^digits(r) factor */
    seq_t factor = 10;
    seq_t tmp = r;
    while (tmp >= 10) {
        tmp /= 10;
        factor *= 10;
    }

    /* Concatenate q and r */
    return q * factor + r;
}
