/*
 * A096039  Triangle read by rows: T(n,k) = (n+1,k)-th element of (M^5-M)/4,
 *          where M is the infinite lower Pascal's triangle matrix, 1<=k<=n.
 * https://oeis.org/A096039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_isqrt_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096039
   Triangle read by rows: T(n,k) = (n+1,k)-th element of (M^5-M)/4,
   where M is the infinite lower Pascal's triangle matrix,
   1 ≤ k ≤ n.  a(n) is the nth element read row‑by‑row (offset 1).

   Indexing: let r be the row number (1‑based) such that
          r*(r-1)/2 < n ≤ r*(r+1)/2
   let c = n - r*(r-1)/2          // column within the row (1‑based)
   The triangle entry is
          T(r,c) = C(r, c-1) * (5^(r+1-c) - 1) / 4
*/

#include <stdint.h>
#include "../utils/floor_isqrt_u64.h"   /* floor_isqrt_u64 */
#include "../utils/gcd.h"              /* gcd */

static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;
        seq_t g   = gcd(num, den);
        num /= g;
        den /= g;
        /* Multiply then divide, keeping intermediate size small */
        res = res * num;
        res = res / den;
    }
    return res;
}

static seq_t pow5(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)5;
    while (exp)
    {
        if (exp & 1U) result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find row r: largest r with r*(r-1)/2 < n */
    idx_t t  = 1 + 8 * n;
    idx_t sq = floor_isqrt_u64(t);   /* floor sqrt */
    idx_t r  = (sq - 1) / 2;
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;

    /* Column within the row */
    idx_t prev = r * (r - 1) / 2;
    idx_t c    = n - prev;            /* 1 ≤ c ≤ r */

    /* Exponent for 5^d  (d = r+1-c) */
    idx_t d = (r + 1) - c;

    /* Compute the triangle entry */
    seq_t power = pow5(d);                     /* 5^d */
    seq_t bin   = binom_idx(r, c - 1);         /* C(r, c-1) */
    seq_t diff  = power - (seq_t)1;            /* 5^d - 1, divisible by 4 */
    seq_t diff_div = diff / (seq_t)4;
    seq_t val = bin * diff_div;                /* T(r,c) */

    return val;
}
