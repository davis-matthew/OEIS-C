/*
 * A153281  Triangle read by rows, A130321 * A127647. Also, number of subsets
 *          of [n+2] with consecutive integers that start at k.
 * https://oeis.org/A153281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(r-k) * Fib(k+1) * where r = floor((sqrt(8*n+1)-1)/2)
 *            (row index), * k = n - r*(r+1)/2 (column index, 0‑based), *
 *            Fib(m) is the Fibonacci number with Fib(0)=0, Fib(1)=1. * * The
 *            implementation uses a binary search to find r, then * obtains
 *            the value by multiplying 2^(r-k) and Fib(k+1)
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/fib.h"

/* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;          /* inclusive upper bound */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* Compute mid*(mid+1)/2 as 128‑bit to avoid overflow. */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;             /* largest mid with tri <= n */
}

seq_t a(idx_t n)
{
    /* Determine row (r) and position (k) within that row. */
    idx_t r = find_row(n);
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;    /* 0‑based column index */

    /* Compute the term: 2^(r-k) * Fib(k+1) */
    seq_t pow_val = pow2(r - k);   /* 2^(r-k) */
    seq_t fib_val = fib(k + 1);    /* Fib(k+1) */

    return pow_val * fib_val;
}
