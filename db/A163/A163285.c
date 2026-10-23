/*
 * A163285  Triangle read by rows in which row n lists n+1 terms, starting
 *          with n^5 and ending with n^6, such that the difference between
 *          successive terms is equal to n^5 - n^4.
 * https://oeis.org/A163285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = r^5 + k*(r^5 – r^4) where r = floor((sqrt(8*n+1) – 1)/2)
 *            (row number) and k = n – r*(r+1)/2 (position in the row)
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 897, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a binary search on triangular numbers and
   computes the 5th power directly, avoiding external utilities. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* n = 0 is a special case: the first row has only one term 0. */
    if (n == 0)
        return 0;

    /* ---------- Locate the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 ---------- */
    idx_t low  = 0;
    idx_t high = 1;
    /* First find an upper bound for r. */
    while ((__uint128_t)high * (high + 1) / 2 <= n)
        high <<= 1;

    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;          /* upper mid */
        if ((__uint128_t)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low; /* row number (r >= 1 for n > 0) */

    /* --------- Compute the position k inside the row ---------- */
    idx_t tri = (idx_t)(((__uint128_t)r * (r + 1)) / 2); /* number of terms before row r */
    idx_t k   = n - tri;                                /* 0‑based index inside the row */

    /* --------- Compute r^5, r^4 and the required value --------- */
    seq_t r2  = (seq_t)r * (seq_t)r;          /* r^2 */
    seq_t r4  = r2 * r2;                      /* r^4 */
    seq_t start = r4 * (seq_t)r;              /* r^5 */
    seq_t diff  = r4 * (seq_t)(r - 1);        /* r^4 * (r-1) = r^5 – r^4 */
    seq_t value = start + (seq_t)k * diff;    /* a(n) */

    return value;
}
