/*
 * A126571  Triangle where the m-th term in row n is the n-th integer from
 *          among those positive integers coprime to m.
 * https://oeis.org/A126571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the r‑th positive integer coprime to m, where r is the
 *            smallest integer with r(r+1)/2 >= n and m = n - r(r-1)/2. The
 *            r‑th coprime is found by brute force
 * Range:     u8 n <= 2375, u16 n <= 9646, u32 n <= 9646, u64 n <= 9646,
 *            u128 n <= 9646, big any n
 * Helpers:   gcd_u64, triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"
#include "../utils/triangular.h"

/* Euclidean algorithm for 64‑bit unsigned integers. */

/* Return the triangular number r(r+1)/2 without overflow. */

/* Find the row number r such that r(r+1)/2 >= n. */
static idx_t find_row(idx_t n) {
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t tri = triangular(mid);
        if (tri >= n) hi = mid; else lo = mid + 1;
    }
    return lo;
}

/* Return the k‑th positive integer coprime to m.  Both k and m are small
   (k <= r <= ~6000 for n up to 2e7, m <= r).  Brute force is fast enough. */
static seq_t kth_coprime(idx_t k, idx_t m) {
    seq_t num = 1;
    idx_t count = 0;
    while (1) {
        if (gcd_u64((uint64_t)m, (uint64_t)num) == 1) {
            ++count;
            if (count == k)
                return num;
        }
        ++num;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined; fallback */

    /* Determine the row (r) and column (m). */
    idx_t r = find_row(n);
    idx_t prev_tri = triangular(r - 1);
    idx_t m = n - prev_tri;               /* 1‑based column index */

    /* Return the r‑th coprime to m. */
    return kth_coprime(r, m);
}
