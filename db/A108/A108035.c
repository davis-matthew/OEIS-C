/*
 * A108035  Triangle read by rows: n-th row consists of n copies of the n-th
 *          nonzero Fibonacci number.
 * https://oeis.org/A108035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value of the Fibonacci sequence with F(1)=1, F(2)=2 (i.e.
 *            standard Fibonacci shifted by one index) at the row r where
 *            r(r+1)/2 ≥ n and (r-1)r/2 < n
 * Range:     u8 n <= 78, u16 n <= 276, u32 n <= 1081, u64 n <= 4278,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find smallest r with r(r+1)/2 >= n */
static inline idx_t row_of_triangle(idx_t n)
{
    idx_t low  = 1;
    idx_t high = 1;

    /* Exponentially increase high until condition holds */
    while ((unsigned __int128)high * (high + 1u) / 2u < (unsigned __int128)n) {
        high <<= 1;
    }

    /* Binary search between low and high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1u) / 2u;
        if (t >= (unsigned __int128)n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 – undefined for 0 */

    idx_t r = row_of_triangle(n);         /* row containing n */

    /* Iterate Fibonacci with F(1)=1, F(2)=2 */
    if (r == 1) return (seq_t)1;
    if (r == 2) return (seq_t)2;

    seq_t prev = (seq_t)1;   /* F(1) */
    seq_t cur  = (seq_t)2;   /* F(2) */

    for (idx_t i = 3; i <= r; ++i) {
        seq_t next = prev + cur;
        prev = cur;
        cur  = next;
    }
    return cur;
}
