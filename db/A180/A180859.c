/*
 * A180859  Square array read by antidiagonals: T(m,n) is the Wiener index of
 *          the windmill graph D(m,n) obtained by taking n copies of the
 *          complete graph K_m with a vertex in common (i.e., a bouquet of n
 *          pieces of K_m graphs; m>=2, n>=1).
 * https://oeis.org/A180859
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(m,n) = n*(m-1)*((m-1)*(2n-1)+1)/2, read by antidiagonals
 * Range:     u8 n <= 31, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   _diag_cum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_diag_cum.h"

seq_t a(idx_t idx)
{
    if (idx < 2) return 0;          /* the sequence is defined for idx ≥ 2 */

    idx_t k = idx - 1;              /* number of terms before the current one */

    /* Find the smallest diagonal S with _diag_cum(S) ≥ k */
    idx_t low = 3, high = 3;
    while (_diag_cum(high) < k) {
        high <<= 1;                 /* double until we are big enough */
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (_diag_cum(mid) >= k)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t S = low;                  /* the diagonal containing the term */

    unsigned __int128 prev = _diag_cum(S - 1); /* terms before this diagonal */
    idx_t pos = k - (idx_t)prev;                /* 1‑based position within the diagonal */
    idx_t m = S - pos;
    idx_t n_local = pos;              /* local variable name to avoid conflict with function parameter */

    /* Compute T(m,n) = n*(m-1)*((m-1)*(2n-1)+1)/2 */
    unsigned __int128 m1 = (unsigned __int128)(m - 1);
    unsigned __int128 n128 = (unsigned __int128)n_local;
    unsigned __int128 inside = m1 * ((unsigned __int128)2 * n128 - 1) + 1;
    unsigned __int128 prod   = n128 * m1 * inside;

    return (seq_t)(prod >> 1);       /* divide by 2 */
}
