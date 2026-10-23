/*
 * A129265  Triangle read by rows: T(n,k) is the number of power of two
 *          divisors of n that are less than or equal to n/k.
 * https://oeis.org/A129265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  OEIS sequence A129265 – Triangle read by rows.
 *  For a linear index n (1‑based) this function returns
 *  a(n) = min(v2(r), floor_log2(r/k)) + 1
 *  where
 *      r = ceil( (-1 + sqrt(1 + 8*n)) / 2 )          // row that contains index n
 *      k = n - (r-1)*r/2                            // column within that row
 *      v2(r) = exponent of 2 in the prime factorisation of r
 *
 *  The sequence is defined by
 *      T(n,k) = number of power‑of‑two divisors of n that are ≤ n/k.
 *  The terms a(1), a(2), … are obtained by flattening T(n,k) by rows.
 *
 *  Complexity: O(log n) per call (binary search for r).
 *  Uses only integer arithmetic and __builtin_* for speed.
 */

#include <stdint.h>
#include <stdbool.h>

/* Find the row r (>=1) such that r*(r+1)/2 >= n.
   r <= ~6.5e9 for n < 2^64, so 128‑bit arithmetic is safe. */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 s = (__int128)mid * (mid + 1) / 2;  /* mid*(mid+1)/2 */
        if (s < n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

/* The requested OEIS sequence function. */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);                                 /* row number */
    idx_t prev_rows = ((__int128)(r - 1) * r) / 2;          /* number of elements in rows before r */
    idx_t k = n - (idx_t)prev_rows;                         /* column within the row */

    idx_t x = r / k;                                        /* floor(r / k) */
    idx_t v2 = __builtin_ctzll(r);                          /* exponent of 2 in r */

    /* floor(log2(x)) – x >= 1 because k <= r */
    idx_t log2x = 63 - __builtin_clzll(x);

    /* minimal exponent + 1 gives the number of suitable powers of two */
    idx_t min_exp = (log2x < v2) ? log2x : v2;
    return (seq_t)(min_exp + 1);
}
