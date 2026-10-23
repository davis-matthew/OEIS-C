/*
 * A143216  Triangle read by rows: T(n,k) = n!*k!, 0 <= k <= n.
 * https://oeis.org/A143216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r! * k! * where r = floor((sqrt(8 n + 1) - 1)/2), * k = n
 *            - r(r + 1)/2 (0 ≤ k ≤ r). * This reads the triangle T(n, k) = n!
 *            * k! row by row
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 52, u64 n <= 103,
 *            u128 n <= 251, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute a(n) for sequence A143216. */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
       Use an incremental search: r grows as ~√(2n). */
    idx_t r = 0;
    while (1) {
        unsigned __int128 nxt = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (nxt > n) break;
        ++r;
    }

    /* Compute the column index k within that row. */
    unsigned __int128 prv = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = (idx_t)(n - (idx_t)prv);

    /* Compute r! (factorial of r). */
    seq_t fact_r = 1;
    for (idx_t i = 2; i <= r; ++i)
        fact_r *= (seq_t)i;

    /* Compute k! (factorial of k). */
    seq_t fact_k = 1;
    for (idx_t j = 2; j <= k; ++j)
        fact_k *= (seq_t)j;

    /* a(n) is the product of the two factorials. */
    return fact_r * fact_k;
}
