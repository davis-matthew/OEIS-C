/*
 * A140256  Triangle read by columns: Column k is A014963 aerated with groups
 *          of (k-1) zeros.
 * https://oeis.org/A140256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(row, col) where row and col are such that * n =
 *            (row-1)*row/2 + col, 1 ≤ col ≤ row. * T(row, col) = 0 if row %
 *            col != 0 * T(row, col) = A014963(row/col) otherwise. *
 *            A014963(m) = m / Π_{d|m, d<m} A014963(d). * By construction
 *            A014963(1)=1 and A014963(m) is always an integer * not excee...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static idx_t isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

static idx_t a014963_max = 0;
static seq_t *a014963 = NULL;

/* Compute A014963 values up to index 'need' (need >= 1) */
static void ensure_A014963(idx_t need) {
    if (need <= a014963_max)
        return;
    seq_t *new_arr = (seq_t *)realloc(a014963, (need + 1) * sizeof(seq_t));
    if (!new_arr) abort();      /* graceful abort on allocation failure */
    a014963 = new_arr;

    for (idx_t m = a014963_max + 1; m <= need; ++m) {
        __uint128_t prod = 1;
        for (idx_t d = 1; d < m; ++d) {
            if (m % d == 0)
                prod *= (unsigned __int128)a014963[d];
        }
        a014963[m] = (seq_t)((seq_t)m / (seq_t)prod);
    }
    a014963_max = need;
}

seq_t a(idx_t n) {
    if (n < 1)
        return 0;     /* safety for out‑of‑range values */

    /* Find the row r such that (r-1)*r/2 < n ≤ r*(r+1)/2 */
    idx_t t = n - 1;
    idx_t disc = (idx_t)(((__uint128_t)8 * t) + 1);      /* 1 + 8*(n-1) */
    idx_t sqrt_disc = isqrt_u64(disc);
    idx_t r = (1 + sqrt_disc) / 2;

    /* Compute column index within this row (1‑based) */
    idx_t offset = (idx_t)(((__uint128_t)(r - 1) * r) / 2) + 1;
    idx_t c = n - offset + 1;

    if (r % c != 0)
        return 0;

    idx_t m = r / c;
    ensure_A014963(m);
    return a014963[m];
}
