/*
 * A126671  Triangle read by rows: row n (n>=0) has g.f. Sum_{i=1..n}
 *          n!*x^i*(1+x)^(n-i)/(n+1-i).
 * https://oeis.org/A126671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^(k‑1) in the polynomial P_{r‑1}(x)
 *            = Σ_{i=1}^{r‑1} (r‑1)! · x^i · (1+x)^{r‑1‑i} / (r‑i) , where r
 *            is the row number (1‑based) and k is the column number
 *            (1‑based). The coefficient is computed by t = (r‑1)! ·
 *            Σ_{i=1}^{min(k‑1,r‑1)} C(r‑1‑i, k‑1‑i) / (r‑i)
 * Range:     u8 n <= 18, u16 n <= 39, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"


static seq_t fact_cache[200];
static idx_t fact_cnt = 0;

/* Ensure factorials 0! … n! are stored in fact_cache */
static void ensure_factorial(idx_t n)
{
    if (n < fact_cnt) return;
    if (fact_cnt == 0) {          /* initialise 0! = 1 */
        fact_cache[0] = (seq_t)1;
        fact_cnt = 1;
    }
    for (idx_t i = fact_cnt; i <= n; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    fact_cnt = n + 1;
}

seq_t a(idx_t N)
{
    if (N < 1) return (seq_t)0;   /* offset */

    /* Find the row r such that r(r+1)/2 ≥ N  (row r has r entries) */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < N) ++r;

    /* Column k (1‑based) within that row */
    idx_t first = r * (r - 1) / 2 + 1;   /* index of first element of row r */
    idx_t k = N - first + 1;            /* 1‑based column */

    idx_t n = r - 1;                    /* parameter for the polynomial */

    ensure_factorial(n);
    seq_t F = fact_cache[n];

    idx_t lim = k - 1;
    if (lim > n) lim = n;

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= lim; ++i) {
        seq_t term = F / (seq_t)(n + 1 - i);               /* (n)!/(n+1-i) */
        seq_t bin  = binom((idx_t)(n - i), (idx_t)(k - 1 - i));
        sum = sum + term * bin;
    }
    return sum;
}
