/*
 * A084980  Triangle of multifactorials: n-th row is (n+1)!!... {n "!"s},
 *          (n+1)!... {n-1 "!"s}, ..., (n+1)!.
 * https://oeis.org/A084980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = multifactorial of (r+1) of order m, where r is the row *
 *            index satisfying r*(r+1)/2 >= n and m = r - (n - r*(r-1)/2) + 1.
 *            * The triangle of multifactorials is built by rows: * Row r has
 *            r terms: (r+1) with r, r-1, …, 1 factorial symbols. * For each
 *            term we compute the product (r+1)*(r+1-m)...
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 77, u64 n <= 209,
 *            u128 n <= 594, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++r;
        sum += r;
    }
    /* Position p within the row (1‑based). */
    idx_t prevSum = sum - r;
    idx_t p = n - prevSum;             /* 1 <= p <= r */

    /* The value to factorial is r+1. */
    seq_t base = (seq_t)(r + 1);

    /* Mult factorial order m = r - p + 1. */
    idx_t m = r - p + 1;
    seq_t step = (seq_t)m;

    /* Compute the multifactorial product. */
    seq_t prod = base;
    seq_t cur = base;
    while (cur > step) {
        cur -= step;
        prod *= cur;
    }
    return prod;
}
