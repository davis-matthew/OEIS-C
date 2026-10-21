/*
 * A079409  Array T(m,n) (m>=0, n>=0) read by antidiagonals: T(0, 0) = 1, T(0,
 *          n) = 0 if n > 0, T(m, n) = T(m-1, n - T(m-1, n)) + T(m-1, n -
 *          T(m-1, n-1)) if m > 0.
 * https://oeis.org/A079409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(m,n) = 0 if n > m * T(m,n) = 1 if n ≤ m and (m‑n) even *
 *            T(m,n) = n + 1 if n ≤ m and (m‑n) odd * The sequence a(n) is the
 *            reading of T(m,n) along antidiagonals * (m+n constant, with m
 *            decreasing within each diagonal). Thus * a(0)=1, a(1)=1, a(2)=0,
 *            a(3)=1, a(4)=1,
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute largest s with s*(s+1)/2 ≤ r.  Uses only 128‑bit arithmetic
   to avoid overflow for idx_t up to 64 bits. */
static inline idx_t find_s(idx_t r)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Expand high until triangular(high) > r */
    while (((unsigned __int128)high * (high + 1)) / 2 <= r)
        high <<= 1;

    /* Binary search for the largest s with triangular(s) ≤ r */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 <= r)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine diagonal index s and position offset o */
    idx_t s = find_s(n);                          /* diagonal sum m+n = s */
    idx_t prev_sum = (idx_t)(((unsigned __int128)s * (s + 1)) / 2);
    idx_t o = n - prev_sum;                       /* 0‑based offset within the diagonal */

    /* Map to coordinates (m,n) with m decreasing in the diagonal */
    idx_t m = s - o;
    idx_t col = o;                                /* column index n = o */

    /* Evaluate T(m, col) according to the closed form */
    if (col > m)
        return (seq_t)0u;

    if (((m - col) & 1u) == 0u)
        return (seq_t)1u;      /* even difference */
    else
        return (seq_t)(col + 1u); /* odd difference */
}
