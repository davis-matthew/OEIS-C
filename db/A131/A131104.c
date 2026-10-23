/*
 * A131104  Rectangular array read by antidiagonals: a(n, k) is the number of
 *          ways to put k labeled objects into n labeled boxes so that there
 *          is one box with exactly one object (n, k >= 1).
 * https://oeis.org/A131104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = entry (i,j) of the rectangular array a(n,k) read by *
 *            antidiagonals, where a(i, j) counts the number of ways to put j
 *            * labeled objects into i labeled boxes so that exactly one box
 *            contains * exactly one object. For i, k ≥ 1, * * f(i, j) = i * j
 *            * g(i-1, j-1) * * where g(m, r) is the numbe...
 * Range:     u8 n <= 32, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   binomial, factorial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/pow_seq.h"

/* Compute g(m, r) as described above.  m, r are idx_t (unsigned). */
static seq_t _g(idx_t m, idx_t r)
{
    if (r == 0) return 1;           /* g(m,0) = 1 for any m */
    seq_t result = 0;
    idx_t max_t = (m < r) ? m : r;
    for (idx_t t = 0; t <= max_t; ++t) {
        seq_t term = binomial(m, t);
        term *= binomial(r, t);
        term *= factorial(t);                      /* t! */
        if (r > t) {
            /* (m - t)^(r - t) */
            term *= pow_seq((seq_t)(m - t), (int)(r - t));
        }
        if (t & 1) result -= term;   /* subtract if t is odd */
        else       result += term;   /* add if t is even */
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */

    /* Find diagonal s such that (s-1)*s/2 >= n, with s >= 2.   */
    idx_t s = 2;
    seq_t cum = 1;                 /* (s-1)*s/2 for s=2 */
    while (cum < n) {
        ++s;
        cum = (seq_t)(s - 1) * s / 2;
    }
    /* cum_prev = (s-2)*(s-1)/2 */
    seq_t cum_prev = (seq_t)(s - 2) * (s - 1) / 2;
    idx_t offset = (idx_t)(n - cum_prev - 1);  /* zero‑based within diagonal */

    /* Coordinates: i = s-1-offset, j = offset+1 */
    idx_t i = s - 1 - offset;  /* row index (boxes) */
    idx_t j = offset + 1;      /* column index (objects) */

    /* f(i, j) = i * j * g(i-1, j-1) */
    if (i == 0 || j == 0) return 0;   /* should never happen for n>=1 */

    seq_t prod = (seq_t)i * (seq_t)j;
    seq_t g_val = _g(i - 1, j - 1);
    prod *= g_val;
    return prod;
}
