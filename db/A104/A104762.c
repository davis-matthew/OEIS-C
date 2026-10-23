/*
 * A104762  Triangle read by rows: row n contains first n nonzero Fibonacci
 *          numbers in decreasing order.
 * https://oeis.org/A104762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = F( r - (n - (r-1)r/2 - 1) ) where r =
 *            ceil((sqrt(8*n+1)-1)/2)
 * Range:     u8 n <= 91, u16 n <= 300, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th term of the triangle: row r consists of the first r nonzero Fibonacci numbers in decreasing order. */

#include <stdint.h>

/* Return the n-th term of A104762. */
seq_t a(idx_t n)
{
    /* Handle the offset: the sequence starts at n = 1. */
    if (n == 0)
        return 0;

    uint64_t N = n;

    /* Find the row r such that r*(r+1)/2 >= N. */
    uint64_t r = 1;
    while ( ((unsigned __int128)r * (r + 1)) / 2 < N )
        ++r;

    /* Number of terms before row r. */
    uint64_t prev = ((unsigned __int128)(r - 1) * r) / 2;

    /* Position inside the row (1-based). */
    uint64_t pos = N - prev; /* pos in 1..r */

    /* Index of the Fibonacci number to output (1-based). */
    uint64_t fib_idx = r - pos + 1;

    /* Compute Fibonacci(Fib_Index) iteratively. */
    seq_t a0 = 0; /* F(0) */
    seq_t b0 = 1; /* F(1) */
    seq_t c;
    for (uint64_t i = 0; i < fib_idx; ++i) {
        /* Compute the next Fibonacci number. */
        c = a0 + b0;
        a0 = b0;
        b0 = c;
    }
    /* a0 now holds F(fib_idx). */
    return a0;
}
