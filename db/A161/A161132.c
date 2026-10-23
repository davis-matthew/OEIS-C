/*
 * A161132  Number of permutations of {1,2,...,n} that have no even fixed
 *          points.
 * https://oeis.org/A161132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{j=0..⌈n/2⌉} d(n‑j) · C(⌈n/2⌉, j) where d(k) are the
 *            derangement numbers (d(0)=1, d(1)=0, d(k)=(k‑1)(d(k‑1)+d(k‑2)))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *der = NULL;          /* der[i] = d(i)  */
static idx_t der_len = 0;          /* number of elements stored (der_len = max_i+1) */
static idx_t der_cap = 0;          /* allocated capacity */

/* make sure der[0..n] are defined */
static void ensure_der(idx_t n)
{
    if (der_len > n) return;

    /* grow array if needed */
    size_t needed = (size_t)(n + 1);
    if (needed > (size_t)der_cap) {
        size_t new_cap = der_cap ? der_cap * 2 : 1;
        while (new_cap < needed) new_cap *= 2;
        der = (seq_t*)realloc(der, new_cap * sizeof(seq_t));
        der_cap = new_cap;
    }

    /* initialise base cases if not done yet */
    if (der_len == 0) {
        der[0] = 1;
        der_len = 1;
    }
    if (n >= 1 && der_len == 1) {
        der[1] = 0;
        der_len = 2;
    }

    /* compute remaining derangements */
    for (idx_t i = der_len; i <= n; ++i) {
        seq_t sum = der[i - 1] + der[i - 2];
        seq_t mult = (seq_t)(i - 1);
        der[i] = sum * mult;
    }
    der_len = n + 1;
}

seq_t a(idx_t n)
{
    /* handle trivial cases quickly */
    if (n <= 1) return 1;           /* a(0)=1, a(1)=1 */

    /* pre‑compute derangements up to n */
    ensure_der(n);

    /* m = ⌈n/2⌉ */
    idx_t m = (n + 1) / 2;

    seq_t result = 0;
    seq_t binom  = 1;               /* C(m,0) */

    for (idx_t j = 0; j <= m; ++j) {
        idx_t k = n - j;            /* term uses d(k) */
        seq_t term = der[k] * binom; /* positive integer */
        result += term;

        if (j == m) break;          /* last iteration, no next binom */
        /* update binom to C(m, j+1) = C(m,j) * (m-j) / (j+1) */
        seq_t num = binom * (seq_t)(m - j);
        binom = num / (seq_t)(j + 1);
    }
    return result;
}
