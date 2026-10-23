/*
 * A193229  A double factorial triangle.
 * https://oeis.org/A193229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) corresponds to the (row‑major) enumeration of the double
 *            factorial triangle T(r,k). For the row r (0‑based) and column k
 *            (0≤k≤r) the element is T(r,k) = (2r‑k)! / ( 2^(r‑k) * (r‑k)! ).
 *            Flattened by rows, a(n) is T(r,k) where the row number r
 *            satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k = n –...
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 170,
 *            u128 n <= 434, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* ---- 1. Determine row r and column k from the index n ---- */
    idx_t r, k;
    {
        /* Solve r(r+1)/2 ≤ n < (r+1)(r+2)/2.
           Use the quadratic solution r = floor((√(1+8n) – 1)/2). */
        long double discr = 1.0L + 8.0L * (long double)n;
        r = (idx_t)((sqrtl(discr) - 1.0L) * 0.5L);

        /* Correct potential round‑off errors. */
        while ( ( (unsigned __int128)(r+1)*(r+2) ) / 2 <= n ) r++;
        while ( ( (unsigned __int128)r*(r+1) ) / 2 > n ) r--;

        k = n - ( (unsigned __int128)r*(r+1) ) / 2;
    }

    /* ---- 2. Compute T(r,k) = (2r‑k)! / ( 2^(r‑k) * (r‑k)! ) ---- */
    idx_t a_val = 2*r - k;          /* upper factorial argument */
    idx_t b_val = r - k;            /* number of 2’s to divide by */

    seq_t result = 1;
    idx_t pow2_rem = b_val;

    /* Multiply the integers from (b_val+1) to a_val, cancelling powers of 2. */
    for (idx_t i = b_val + 1; i <= a_val; ++i) {
        seq_t x = (seq_t)i;
        while (pow2_rem > 0 && (x & 1) == 0) {
            x >>= 1;
            pow2_rem--;
        }
        result *= x;
    }

    /* Any remaining division by 2^(pow2_rem).  It is exact. */
    if (pow2_rem > 0)
        result >>= pow2_rem;

    return result;
}
