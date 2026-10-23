/*
 * A167546  The ED1 array read by antidiagonals.
 * https://oeis.org/A167546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom, factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A167546 – ED1 array read by antidiagonals.  
 * 
 * Formula:
 *   a(n,m) = 4^(m-1) * (m-1)! * (n+m-2)! / (2*m-2)!      for m ≤ n
 *   a(n,m) satisfies  
 *     Σ_{k=0}^{n-1} (-1)^k * binom(n-1,k) * a(n,m-k) = (2n-1)(n-1)!  for m > n
 *
 * The public sequence a(idx_t n) is the lexicographic reading of the
 * array by antidiagonals (row i, column j ↦ sequence index n).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/* utilities */
#include "../utils/factorial.h"    /* seq_t factorial(idx_t n); */
#include "../utils/binom.h"        /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/seq_pow.h"      /* seq_t seq_pow(seq_t base, idx_t exp); */

/* ------------------------------------------------------------------ */
/* Integer square‑root for 64‑bit unsigned values (via binary search). */
static idx_t
isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = ((idx_t)1 << 32);          /* 2^32 > sqrt(2^64)        */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * mid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;                         /* floor(sqrt(n))            */
}

/* ------------------------------------------------------------------ */
/* Compute a(n,m) for n ≥ 1 and m ≥ 1.  Indexes are 1‑based.          */
static seq_t
a_pair(idx_t n, idx_t m)
{
    /* A safe upper bound for n, m that will be needed for the 10 k test. */
    #define MAXLEN 2000
    static seq_t row[MAXLEN + 1];   /* indices 1 .. MAXLEN  */

    /* RHS constant for the recurrence (2*n-1)*(n-1)! */
    seq_t rhs = ((seq_t)(2 * n - 1)) * factorial(n - 1);

    for (idx_t mm = 1; mm <= m; ++mm) {
        if (mm <= n) { /* direct formula for the lower‑left triangle */
            /* 4^(mm-1) */
            seq_t pow4 = seq_pow((seq_t)4, (idx_t)(mm - 1));
            /* (mm-1)! */
            seq_t fact_m1 = factorial(mm - 1);
            /* product from (2*mm-1) to (n+mm-2)  (empty when start > end) */
            seq_t prod = (seq_t)1;
            idx_t start = 2 * mm - 1;
            idx_t end   = n + mm - 2;
            for (idx_t t = start; t <= end; ++t) {
                prod *= (seq_t)t;
            }
            row[mm] = pow4 * fact_m1 * prod;
        } else {          /* recurrence for the upper‑right triangle   */
            seq_t term_sum = (seq_t)0;
            for (idx_t k = 1; k <= n - 1; ++k) {
                seq_t coeff = binom(n - 1, k);
                seq_t prod  = coeff * row[mm - k];
                if (k & 1)      /* (-1)^k ; odd k → subtract */
                    term_sum -= prod;
                else            /* even k   → add            */
                    term_sum += prod;
            }
            row[mm] = rhs - term_sum;       /* a(n,mm) = rhs – Σ_{k≥1} …   */
        }
    }
    return row[m];
#undef MAXLEN
}

/* ------------------------------------------------------------------ */
/* Public function: read the array by antidiagonals.                   */
seq_t
a(idx_t n)
{
    if (n == 0)          /* offset is 1; this value is never requested    */
        return (seq_t)0;

    /* Find the unique diagonal t = i + j (1‑based) such that
     *   t(t+1)/2 - t  <  n ≤ t(t+1)/2
     * Equivalent: smallest t with t(t-1)/2 ≥ n.
     */
    idx_t D  = 1 + 8 * n;                          /* 8n+1  */
    idx_t sq = isqrt_u64(D);                       /* floor(sqrt) */
    idx_t t  = (sq + 1) / 2;
    if (t * (t - 1) / 2 < n)
        ++t;

    idx_t prev = (t - 1) * (t - 2) / 2;            /* prior diagonal size  */
    idx_t offset = n - prev - 1;                  /* 0‑based inside diagonal */

    idx_t i = t - 1 - offset;                     /* row index  (1‑based)  */
    idx_t j = 1 + offset;                         /* column index (1‑based) */

    return a_pair(i, j);
}
