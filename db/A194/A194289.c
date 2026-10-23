/*
 * A194289  Triangular array: g(n,k)=number of fractional parts (i*sqrt(3)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n, 1<=k<=n.
 * https://oeis.org/A194289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the triangular array
   g(n,k)=# { 1≤i≤n : fractional part of i·√3 lies in [(k-1)/n , k/n) }.
   The OEIS sequence A194289 is the row‑wise flattening of that array
   with offset 1.  The function computes a(n) by finding the row r
   and column k corresponding to the index n and then counting how many
   i∈[1,r] satisfy the interval condition.  No floating point arithmetic
   is used: the test is performed with integer square roots only. */
#include <stdint.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* 1. Find the row r such that r·(r+1)/2 ≥ n */
    idx_t t = int_sqrt(8 * n + 1);        /* floor( sqrt(8n+1) ) */
    idx_t r = (t - 1) >> 1;              /* floor( (sqrt(8n+1)-1)/2 ) */
    if ((r * (r + 1)) / 2 < n)
        ++r;                             /* adjust if we undershot */

    /* 2. Locate the column k within that row */
    idx_t start = (r * (r - 1)) / 2 + 1; /* first index of row r */
    idx_t k = n - start + 1;             /* 1‑based column number */

    /* 3. Count i with floor(i·√3·r) mod r == k‑1 */
    seq_t count = 0;
    for (idx_t i = 1; i <= r; ++i)
    {
        /* Compute floor( i·√3·r ) without using √3 directly. */
        unsigned __int128 prod = (unsigned __int128)3 * i * i * r * r;
        idx_t q = int_sqrt((uint64_t)prod);   /* floor( sqrt(3·i²·r²) ) */

        if ((q % r) == (k - 1))
            ++count;
    }
    return count;
}
