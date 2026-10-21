/*
 * A093423  Consider the triangle whose first part is shown as an example in
 *          the entry A093422. If the n-th term of the triangle read by rows
 *          is a fraction then a(n) is the denominator of the fraction,
 *          otherwise a(n)=1.
 * https://oeis.org/A093423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = denominator of the reduced fraction 2·C(r,m)·(m‑1)!/(2·r
 *            – m + 1) read by rows; r is the row index, m is the column
 *            (1‑based). For m = 1 the fraction equals r, so a(n)=1
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"

/* Count the exponent of prime p in n! = ∏_{k≥1} ⌊n/p^k⌋ */
static idx_t vp_fact(idx_t n, idx_t p)
{
    idx_t exp = 0;
    while (n)
    {
        n /= p;
        exp += n;
    }
    return exp;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, defensive */

    /* locate row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((seq_t)r * (r + 1) / 2 < n)
        ++r;

    idx_t prev = (r - 1) * r / 2;        /* number of terms before row r */
    idx_t m    = n - prev;               /* column index in this row (1‑based) */

    if (m == 1)                          /* first column, integer value r */
        return (seq_t)1;

    /* denominator before reduction */
    seq_t denom = (seq_t)(2 * r - m + 1);

    /* Compute gcd( N, denom ) by prime‑factor analysis of denom.
       N = 2 * C(r,m) * (m-1)!  and  denom = 2r-m+1   (<= 300)    */
    seq_t gcd_val = (seq_t)1;
    int tempD = (int)(2 * r - m + 1);    /* fits into int comfortably */

    for (int p = 2; p * p <= tempD; ++p)
    {
        if (tempD % p == 0)
        {
            int eD = 0;
            while (tempD % p == 0) { tempD /= p; ++eD; }

            int eN = 0;
            if (p == 2) eN += 1;                       /* factor 2 from N */
            eN += (int)vp_fact(r, p) - (int)vp_fact(m, p) - (int)vp_fact(r - m, p);
            eN += (int)vp_fact(m - 1, p);

            int eG = eN < eD ? eN : eD;
            for (int i = 0; i < eG; ++i)
                gcd_val = seq_mul(gcd_val, (seq_t)p);
        }
    }
    if (tempD > 1)                         /* remaining prime factor > 1 */
    {
        int p = tempD;
        int eD = 1;

        int eN = 0;
        if (p == 2) eN += 1;
        eN += (int)vp_fact(r, p) - (int)vp_fact(m, p) - (int)vp_fact(r - m, p);
        eN += (int)vp_fact(m - 1, p);

        int eG = eN < eD ? eN : eD;
        for (int i = 0; i < eG; ++i)
            gcd_val = seq_mul(gcd_val, (seq_t)p);
    }

    return denom / gcd_val;                /* reduced denominator */
}
