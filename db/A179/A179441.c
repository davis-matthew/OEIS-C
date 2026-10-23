/*
 * A179441  Number of solutions to a+b+c < d+e with each of a,b,c,d,e in
 *          {1..n+1}.
 * https://oeis.org/A179441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (27*n^5 + 80*n^4 + 65*n^3 - 20*n^2 - 32*n) / 120
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 113, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the closed‑form expression for A179441, which is valid for all n >= 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* values for n < 1 are undefined; return 0 */
    
    /* use an unsigned 128‑bit accumulator to avoid intermediate overflow */
    unsigned __int128 nn  = (unsigned __int128)n;
    unsigned __int128 n2  = nn * nn;
    unsigned __int128 n3  = n2 * nn;
    unsigned __int128 n4  = n3 * nn;
    unsigned __int128 n5  = n4 * nn;
    
    unsigned __int128 poly = 27ULL * n5
                          + 80ULL * n4
                          + 65ULL * n3
                          - 20ULL * n2
                          - 32ULL * nn;
                          
    unsigned __int128 res  = poly / 120ULL;   /* division is exact */

    return (seq_t)res;
}
