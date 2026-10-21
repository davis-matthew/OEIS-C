/*
 * A055630  Table T(k,m) = k^2 + m read by antidiagonals.
 * https://oeis.org/A055630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = k^2 + m where k = s - p, m = p, s = floor((√(8·n+1) −
 *            1)/2), p = n − s·(s+1)/2 (antidiagonal enumeration of
 *            T(k,m)=k^2+m)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t triangular(idx_t s) {
    /* Compute s*(s+1)/2 using 128‑bit arithmetic to avoid overflow. */
    return (idx_t)((__int128)s * (s + 1) / 2);
}

seq_t a(idx_t n) {
    /* Find the diagonal index s such that T(s) <= n < T(s+1). */
    idx_t low = 0, high = n, s = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = triangular(mid);
        if (tri <= n) {
            s = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    idx_t prev = triangular(s);          /* T(s) = s*(s+1)/2            */
    idx_t p    = n - prev;                /* position within the diagonal */
    idx_t k    = s - p;                   /* descending k along the diagonal */
    idx_t m    = p;

    return (seq_t)k * (seq_t)k + (seq_t)m;  /* k^2 + m */
}
