/*
 * A051601  Rows of triangle formed using Pascal's rule except we begin and
 *          end the n-th row with n.
 * https://oeis.org/A051601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 178, u32 n <= 611, u64 n <= 2307,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A051601 (rows of a Pascal‑like triangle)
    a(n) = binomial(r, k+1) + binomial(r, r-k+1)
    where r is the unique integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
    (i.e. n lies in the r‑th row) and k = n – r*(r+1)/2.
    Each row has length r+1 and starts and ends with the value r. */

#include <stdint.h>
#include <stdbool.h>

/* Find largest r such that r*(r+1)/2 ≤ n. */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0, high = n;           /* r never exceeds n */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;  /* last mid with mid*(mid+1)/2 ≤ n */
}

/* Exact binomial coefficient using 128‑bit intermediate to avoid
   overflow for the 64‑bit build; truncation is harmless for values
   that do not fit into seq_t. */
static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n - k)          /* use symmetry */
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res is guaranteed to divide (n+1-i) exactly at each step */
        res = (seq_t)((__int128)res * (n + 1 - i) / i);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* locate the row r and column k within that row */
    idx_t r = find_row(n);
    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri;

    /* compute the two binomial terms; overflow is fine for terms
       that exceed seq_t capacity */
    seq_t left  = binom(r, k + 1);
    seq_t right = binom(r, r - k + 1);
    return left + right;
}
