/*
 * A233558  Triangle read by rows: T(n,k) = real part mod n of (n + ki)^2,
 *          where k=1..n-1 and i is the imaginary unit.
 * https://oeis.org/A233558
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle T(r,k) = (r^2 - k^2) mod r for k=1..r-1, read row‑wise
 *            starting with r=2. For a given OEIS index n (n≥2) let m = n-2.
 *            Find the row index r = ceil( (1+√(1+8m)) / 2 ), then k = m -
 *            (r-1)(r-2)/2 + 1. Then a(n) = T(r,k)
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n) {
    /* n ≥ 2 by definition of the sequence */
    uint64_t m = n - 2;  /* 0‑based flat index */
    /* Compute row r */
    double d = sqrt((double)1 + 8.0 * (double)m);
    uint64_t r = (uint64_t)((1.0 + d) / 2.0);   /* floor of the root */
    /* Adjust upwards until r(r-1)/2 > m */
    while ((unsigned __int128)r * (r - 1) / 2 <= m)
        r++;

    /* Find offset within the row */
    uint64_t prev = (unsigned __int128)(r - 1) * (r - 2) / 2;
    uint64_t offset = m - prev;   /* 0‑based position in row */
    uint64_t k = offset + 1;      /* k runs from 1 to r-1 */

    /* Compute (r^2 - k^2) mod r */
    seq_t r_val = (seq_t)r;
    seq_t k_val = (seq_t)k;
    seq_t val = (r_val * r_val - k_val * k_val) % r_val;

    return val;
}
