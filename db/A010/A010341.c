/*
 * A010341  Sum of all terms except last in period of continued fraction for
 *          sqrt(n).
 * https://oeis.org/A010341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all terms except the last in the period of the *
 *            continued‑fraction expansion of sqrt(n)
 * Range:     u8 n <= 1365, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root: floor(sqrt(n)), for arbitrary 64‑bit n. */
static idx_t isqrt_floor(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t a0 = isqrt_floor(n);

    /* Perfect square: no periodic terms. */
    if (a0 * a0 == n) {
        return sum;
    }

    idx_t m = 0;
    idx_t d = 1;
    idx_t a_curr = a0;          /* a0 is not part of the period. */
    idx_t two_a0 = a0 + a0;     /* 2*a0 for comparison. */

    while (1) {
        idx_t m_next = d * a_curr - m;
        idx_t d_next = (n - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Last term of the period equals 2*a0; stop before adding it. */
        if (a_next == two_a0) {
            break;
        }

        sum += (seq_t)a_next;

        m      = m_next;
        d      = d_next;
        a_curr = a_next;
    }

    return sum;
}
