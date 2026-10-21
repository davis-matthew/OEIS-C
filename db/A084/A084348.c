/*
 * A084348  Triangle read by rows: T(n,k) = floor(e*k!) - n*floor(e*k!/n).
 * https://oeis.org/A084348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(e*k!)-r*floor(e*k!/r) where r is the row number and
 *            k = n - r(r-1)/2 is the column number in the triangle read by
 *            rows. The result is congruent to (floor(e*k!)) modulo r, i.e.
 *            a(n) = (floor(e*k!)) mod r
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t *suffix = NULL;
static size_t suffix_cap = 0;

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n  */
    idx_t r = 0, cum = 0;
    while (cum < n) {
        ++r;
        cum += r;
    }
    /* k = column index (1‑based) inside row r */
    idx_t k = n - (cum - r);

    /* Ensure we have enough space for suffix array */
    if ((size_t)(k + 2) > suffix_cap) {
        free(suffix);
        suffix = (idx_t*)malloc((k + 2) * sizeof(idx_t));
        suffix_cap = k + 2;
    }

    /* suffix[t] = product_{i=t}^{k} i  (mod r),  t = 1..k+1   */
    suffix[k + 1] = 1 % r;               /* empty product */
    for (idx_t t = k; t >= 1; --t) {
        suffix[t] = (suffix[t + 1] * t) % r;
    }

    /* Compute Σ_{m=0}^{k} suffix[m+1]  modulo r */
    idx_t sum_mod = 0;
    for (idx_t m = 0; m <= k; ++m) {
        sum_mod += suffix[m + 1];
        if (sum_mod >= r) sum_mod %= r;  /* keep within bounds */
    }

    return (seq_t)sum_mod;               /* a(n) is < r */
}
