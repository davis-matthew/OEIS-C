/*
 * A074147  (2n-1) odd numbers followed by 2n even numbers.
 * https://oeis.org/A074147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 248, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A074147: (2n‑1) odd numbers followed by 2n even numbers.
   a(n) = start(r) + 2*t
   where r is the unique integer with r(r‑1)/2 < n ≤ r(r+1)/2,
   t = n – r(r‑1)/2 – 1 (0‑based index within the row).
   For odd r      start(r) = 2*((r‑1)/2)² + 1  (first odd of the row).
   For even r     start(r) = 2*((r‑2)/2)*((r)/2)+2  (first even of the row). */

#include <stdint.h>   /* for uint64_t used in idx_t */

/* helper: find the row r containing the n‑th term */
static idx_t row_of_n(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* double hi until it satisfies hi*(hi+1)/2 ≥ n */
    while ((unsigned __int128)hi * (hi + 1u) / 2u < n)
        hi <<= 1u;
    /* binary search for minimal r */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2u;
        if ((unsigned __int128)mid * (mid + 1u) / 2u >= n)
            hi = mid;
        else
            lo = mid + 1u;
    }
    return lo;
}

/* sequence definition */
seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;   /* defined only for n ≥ 1; return 0 for n=0 */

    idx_t r   = row_of_n(n);
    idx_t pre = (unsigned __int128)(r - 1u) * r / 2u;   /* terms before row r */
    idx_t t   = n - pre - 1u;                           /* 0‑based index in row */

    if (r & 1u) {   /* odd row – odd numbers */
        idx_t k = (r - 1u) / 2u;
        seq_t start = (seq_t)(2 * (unsigned __int128)k * k + 1);
        return start + (seq_t)(t * 2u);
    } else {        /* even row – even numbers */
        idx_t k = (r - 2u) / 2u;
        seq_t start = (seq_t)(2 * ((unsigned __int128)k * (k + 1u) + 1u));
        return start + (seq_t)(t * 2u);
    }
}
