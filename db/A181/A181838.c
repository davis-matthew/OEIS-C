/*
 * A181838  T(n,p) = [p prime and is strongly prime to n], the indicator
 *          function of strongly coprime primes, triangle read by rows.
 * https://oeis.org/A181838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Validated: partial, pass, 127 terms through n = 126 (2026-11-01)
 * Helpers:   gcd_idx, ipow_i32, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th flattened element of the triangle Lucc a(n) defined by
   T(r,c)=1 if the column index c is a prime that is coprime with row
   index r and does not divide r-1; otherwise T(r,c)=0.
   The flattened index n satisfies: r(r+1)/2 ≤ n < (r+1)(r+2)/2,
   with c = n - r(r+1)/2. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_idx.h"
#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Determine the row r and column c from flat index n */
    /* Compute r ≈ (sqrt(8n+1)-1)/2 using integer arithmetic */
    seq_t s = (seq_t)n * 8 + 1;          /* 8*n + 1, fits in seq_t */
    seq_t sq = floor_sqrt(s);           /* floor(sqrt(8n+1)) */
    idx_t r = (idx_t)((sq - 1) / 2);     /* initial guess for row */

    /* Correct r for possible rounding errors */
    idx_t start = r * (r + 1) / 2;       /* start index of row r */
    while (start > n) {
        --r;
        start = r * (r + 1) / 2;
    }
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
        start = r * (r + 1) / 2;
    }

    idx_t c = n - start;                /* column index in row r */

    /* Evaluate T(r,c) */
    if (!is_prime(c))                  /* column must be prime */
        return (seq_t)0;

    if (gcd_idx(r, c) != 1)     /* coprime requirement */
        return (seq_t)0;

    if (r > 0 && (r - 1) % c == 0)      /* c must not divide r-1 */
        return (seq_t)0;

    return (seq_t)1;
}
