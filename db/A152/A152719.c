/*
 * A152719  Triangle read by rows: T(n,k) = A000129( 1 + min(k,n-k) ), n>=0,
 *          0<=k<=n.
 * https://oeis.org/A152719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 363, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152719:  triangle of Pell numbers read row‑wise.
   a(n) = P(1 + min(k, r-k)),
   where r is the unique row such that r*(r+1)/2 <= n < (r+1)*(r+2)/2,
   and k = n - r*(r+1)/2 is the column index within that row.
   Pell numbers are defined by P(1)=1, P(2)=2, P(n)=2*P(n-1)+P(n-2). */
 
#include <stdint.h>
 
seq_t a(idx_t n)
{
    /* find the row r containing the n‑th element */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2)) / 2 <= n )
        ++r;
 
    /* index of the first element of this row */
    idx_t prev = r * (r + 1) / 2;
 
    /* column index within the row (0‑based) */
    idx_t k = n - prev;
 
    /* compute the Pell index m = 1 + min(k, r-k) */
    idx_t m = (k <= r - k) ? k + 1 : (r - k) + 1;
 
    /* evaluate Pell(m) */
    if (m == 1)
        return (seq_t)1;
    if (m == 2)
        return (seq_t)2;
 
    seq_t p0 = 1;   /* P(1) */
    seq_t p1 = 2;   /* P(2) */
    seq_t p2;
    for (idx_t i = 3; i <= m; ++i) {
        p2 = 2 * p1 + p0;
        p0 = p1;
        p1 = p2;
    }
    return p1;  /* p1 holds P(m) */
}
