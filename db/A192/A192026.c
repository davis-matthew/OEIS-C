/*
 * A192026  Square array read by antidiagonals: W(n,m) (n >= 3, m >= 1) is the
 *          Wiener index of the graph G(n,m) obtained from an n-wheel graph by
 *          adjoining m pendant edges at each node of the cycle.
 * https://oeis.org/A192026
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = n*(n-1)*(m+1)*(2*m+1) where (n,m) is obtained by
 *            enumerating the pairs with n >= 3, m >= 1 in antidiagonal order:
 *            increasing sums s = n + m, and for each s listing the pairs with
 *            decreasing n (i.e., increasing m). The sequence offset is 3
 *            (a(3) is the first term). This function returns a(n...
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper: cumulative count of pairs with n+m <= s */
static inline unsigned __int128 cum(idx_t s)
{
    return (unsigned __int128)(s - 3) * (s - 2) / 2;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* undefined for n < 3, return 0 */

    idx_t pos = n - 2;            /* 1‑based position in the antidiagonal order */

    /* binary search for smallest s >= 4 s.t. cum(s) >= pos */
    idx_t low = 4;
    idx_t high = low;
    while (cum(high) < pos) high <<= 1;      /* ensure high is large enough */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum(mid) >= pos)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    idx_t count_before = (idx_t)cum(s - 1);   /* pairs before diagonal s */
    idx_t offset = pos - count_before - 1;    /* 0‑based index inside the diagonal */

    idx_t n_pair = (s - 1) - offset;          /* actual n value */
    idx_t m_pair = offset + 1;                /* actual m value */

    seq_t val = (seq_t)n_pair * (seq_t)(n_pair - 1);
    val *= (seq_t)(m_pair + 1) * (seq_t)(2 * m_pair + 1);
    return val;
}
