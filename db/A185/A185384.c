/*
 * A185384  A binomial transform of Fibonacci numbers.
 * https://oeis.org/A185384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r,k) * F(2r - k + 1) where r is the unique
 *            integer with r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n -
 *            r*(r+1)/2. F(m) is the Fibonacci sequence with F(1)=1, F(2)=1,
 *            F(n)=F(n-1)+F(n-2). This yields the triangular array flattened
 *            in row-major order
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 157, u64 n <= 569,
 *            u128 n <= 2098, big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/fib.h"       /* fib(idx_t n) */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       Since n is small (n ≤ 10_000 in tests), a linear scan is
       fast and avoids floating‑point or sqrt. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index k within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t k     = n - start;

    /* Fibonacci index for this cell */
    idx_t fib_index = 2 * r - k + 1;   /* guaranteed ≥ 1 */

    /* Compute the term */
    seq_t bin = binomial(r, k);        /* binomial coefficient */
    seq_t fibv = fib(fib_index);      /* Fibonacci number */
    return bin * fibv;                /* product */
}
