/*
 * A240857  Triangle read by rows: T(0,0) = 0; T(n+1,k) = T(n,k+1), 0 <= k <
 *          n; T(n+1,n) = T(n,0); T(n+1,n+1) = T(n,0)+1.
 * https://oeis.org/A240857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A048881(n_row + n_col) where n_row =
 *            floor((sqrt(1+8*n)-1)/2), n_col = n - n_row*(n_row+1)/2
 * Range:     u8 n <= 8000, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute sequence A240857 */
#include <math.h>
#include <stdint.h>  /* for NULL */

seq_t a(idx_t n)
{
    /* 1. Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt((double)8.0*n + 1.0) - 1.0) / 2.0);

    /* Correct possible off‑by‑one caused by double rounding */
    while (( (__int128)(r + 1) * (r + 2) / 2 ) <= n) ++r;
    while (( (__int128)r * (r + 1) / 2 ) > n) --r;

    /* 2. Column within that row */
    idx_t start = (idx_t)(( (__int128)r * (r + 1) / 2 ));
    idx_t col   = n - start;

    /* 3. Argument of A048881: r + col */
    idx_t s = r + col;

    /* 4. Compute A048881(s) by the recurrence
           A(0)=0,  A(x)=0 if x = 2^k - 1,
           otherwise A(x)=A(x-2^k)+1 where 2^k is the highest power of two ≤ x */
    seq_t result = 0;
    while (s > 0) {
        /* highest power of two ≤ s */
        idx_t msb = 1ULL << (63 - __builtin_clzll(s));
        idx_t rest = s - msb;
        if (rest == msb - 1)        /* x = 2^k - 1 -> value 0 */
            break;
        result += 1;                /* one step in the recurrence */
        s = rest;                    /* continue with the reduced argument */
    }
    return result;
}
