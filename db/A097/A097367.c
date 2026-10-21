/*
 * A097367  Fibonacci regression array: For n>=2 and 1<=k<=n-1, T(n,k) is the
 *          last term before the first nonpositive term in the sequence n, k,
 *          n-k, 2k-n, 2n-3k, 5k-3n, ...
 * https://oeis.org/A097367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A097367 – Fibonacci regression array  
    a(n) is the last positive term in the sequence
    d1 = r , d2 = k , dj = d(j-2) − d(j-1)
    where (r,k) are obtained from n by

    r = min{ r | r(r‑1)/2 ≥ n }
    k = n – (r‑1)(r‑2)/2     (1 ≤ k ≤ r‑1)
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)          /* required only for n ≥ 1; behaviour otherwise is irrelevant */
        return (seq_t)0;

    /* ---- find the row r and column k corresponding to index n ---- */
    idx_t r = 1;
    /* use 128‑bit arithmetic to avoid overflow for very large n */
    while (( (__uint128_t)r * (r - 1) / 2 ) < n)
        ++r;

    /* number of terms before this row */
    idx_t before = (idx_t)( (__uint128_t)(r - 1) * (r - 2) / 2 );
    idx_t k = n - before;            /* 1 ≤ k ≤ r‑1 */

    /* ---- compute T(r,k) : last positive term of the recurrence ---- */
    /* use signed 64‑bit for intermediate values (r and k are small for
       the required limits, but this works for much larger values as well) */
    int64_t d1 = (int64_t)r;
    int64_t d2 = (int64_t)k;
    int64_t last = d1;   /* d1 is positive, so initialise with it */

    while (1) {
        int64_t d3 = d1 - d2;   /* dj = d(j-2) – d(j-1) */
        if (d3 <= 0)
            break;               /* first non‑positive term encountered */
        last = d3;               /* keep the last positive term */
        d1 = d2;
        d2 = d3;
    }

    return (seq_t)last;
}
