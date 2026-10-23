/*
 * A117875  Triangle a(n,m) = (A000045(m) mod 3)+ (A000045(n) mod 7) read by
 *          rows.
 * https://oeis.org/A117875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (fib(row(n)) mod 7) + (fib(col(n)) mod 3) where row(n) is
 *            the unique k with k(k+1)/2 <= n < (k+1)(k+2)/2 and col(n) = n -
 *            k(k+1)/2. The triangle entry a(n,m) = (fib(m) mod 3)+(fib(n) mod
 *            7)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fibonacci numbers modulo 3 and 7; their Pisano periods are 8 and 16 respectively. */
static const seq_t FIB_MOD3[8]  = {0,1,1,2,0,2,2,1};
static const seq_t FIB_MOD7[16] = {0,1,1,2,3,5,1,6,0,6,6,5,4,2,6,1};

/* Find the row index r such that r(r+1)/2 <= n < (r+1)(r+2)/2.  */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0,
          high = ((idx_t)1 << 32);          /* upper bound, exclusive */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* mid <= 2^32-1, so the product fits in uint64_t. */
        unsigned __int128 s = (unsigned __int128)mid * (mid + 1) / 2;
        if (s <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    idx_t row = find_row(n);
    idx_t base = row * (row + 1) / 2;   /* first index of this row */
    idx_t col  = n - base;

    seq_t frow = FIB_MOD7[row % 16];
    seq_t fcol = FIB_MOD3[col % 8];

    return frow + fcol;
}
