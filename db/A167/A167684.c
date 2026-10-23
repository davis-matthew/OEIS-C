/*
 * A167684  Triangle read by rows given by [2,-1,1,-2,0,0,0,0,0,0,0,...] DELTA
 *          [1,0,1,-1,0,0,0,0,0,0,0,...] where DELTA is the operator defined
 *          in A084938.
 * https://oeis.org/A167684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n+1)-st element of the triangle defined by
 *            T(0,0)=1, T(1,0)=T(2,0)=2, T(1,1)=T(2,2)=1, T(2,1)=3,
 *            T(n,k)=T(n-1,k-1) for n>2; all other positions are 0. a(n) =
 *            T(r,c) where r = floor((sqrt(8*n+1)-1)/2) (row index) c = n -
 *            r*(r+1)/2 (column index)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* integer square‑root using long double for a quick initial guess,
   then correcting to guarantee floor(sqrt(x)) */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t r = (idx_t)sqrt((long double)x);
    unsigned __int128 t;

    /* adjust up if necessary */
    for (;;) {
        t = (unsigned __int128)(r + 1) * (r + 1);
        if (t > x) break;
        r++;
    }
    /* adjust down if necessary */
    for (;;) {
        t = (unsigned __int128)r * r;
        if (t <= x) break;
        r--;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* find row r and column c of the triangle */
    idx_t t  = 8 * n + 1;
    idx_t sr = floor_sqrt_u64(t);
    idx_t r  = (sr - 1) / 2;                 /* row index, 0‑based */

    /* triangular number r*(r+1)/2 – use 128 bits to avoid overflow */
    unsigned __int128 tri128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t tri = (idx_t)tri128;

    idx_t c = n - tri;                       /* column index, 0‑based */

    /* compute the value according to the recurrence and base cases */
    if (r == 0)                 return (seq_t)1;

    if (r == 1)                 return (c == 0 ? (seq_t)2 : (seq_t)1);

    if (r == 2)                 return (c == 0 ? (seq_t)2 :
                                    (c == 1 ? (seq_t)3 : (seq_t)1));

    /* for r > 2: T(r,k) = T(2, k-(r-2)) if the shifted index is ≥0; */
    idx_t kprime = c + 2 - r;          /* = c - (r-2) */
    if (kprime == 0)                 return (seq_t)2;
    if (kprime == 1)                 return (seq_t)3;
    if (kprime == 2)                 return (seq_t)1;
    return (seq_t)0;
}
