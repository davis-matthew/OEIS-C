/*
 * A128924  T(n,m) is the number of m's in the fundamental period of Fibonacci
 *          numbers mod n.
 * https://oeis.org/A128924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th entry of the triangle T(n,m) * where T(n,m)
 *            counts how many times the residue m * appears in the fundamental
 *            (Pisano) period of Fibonacci * numbers modulo n. The triangle is
 *            read by rows * (0 ≤ m < n), so row r has r entries. The function
 *            * thus finds the row r such that * r(r‑1)...
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Validated: standard, pass, 121 terms through n = 6895 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count how many times residue m (0 ≤ m < mod) appears
 * in the Pisano period modulo mod.  The count fits
 * in seq_t. */
static seq_t count_in_period(idx_t mod, idx_t m)
{
    if (mod == 1) {
        /* Modulo 1: the period consists of a single 0. */
        return (m == 0) ? 1 : 0;
    }

    idx_t a = 0;
    idx_t b = 1 % mod;               /* ensure b < mod */
    seq_t cnt = 0;

    while (true) {
        if (a == m) {
            cnt++;
        }
        idx_t tmp = a + b;
        if (tmp >= mod) tmp -= mod;   /* tmp < 2*mod, so one subtraction is enough */
        a = b;
        b = tmp;
        if (a == 0 && b == (1 % mod))
            break;
    }
    return cnt;
}

/* Find the smallest r such that r*(r+1)/2 >= n.
 * n is 1‑based and n >= 1.  r fits in idx_t.  */
static idx_t find_row(idx_t n)
{
    /* Use a doubling search for the upper bound.  */
    idx_t lo = 1;
    idx_t hi = 1;
    while (1) {
        __int128 t = (__int128)hi * (hi + 1) / 2;
        if (t >= n) break;
        if (hi > (UINT64_MAX >> 1)) {   /* avoid overflow */
            hi = UINT64_MAX;
            break;
        }
        hi <<= 1;
    }
    /* binary search between lo and hi */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Row r such that n lies in that row. */
    idx_t r = find_row(n);

    /* Start index of row r in the flattened triangle:
     * sum of lengths of rows 1..(r-1) = r*(r-1)/2.  */
    __int128 start_full = (__int128)r * (r - 1) / 2;
    idx_t start = (idx_t)start_full;          /* fits in idx_t for our constraints */

    /* Column index (0‑based) within the row. */
    idx_t k = n - start - 1;   /* 0 ≤ k < r */

    return count_in_period(r, k);
}
