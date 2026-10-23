/*
 * A038097  Number of rooted connected graphs on n labeled nodes where the
 *          root has degree 3.
 * https://oeis.org/A038097
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * a′_n (a′_n = number of connected graphs on n labeled
 *            * vertices rooted at a fixed vertex of degree 3). a′_n satisfies
 *            * the recurrence * * a′_n = G_n – Σ_{k=1}^{n-1} C(n‑1, k‑1) ·
 *            a′_k · 2^{C(n‑k, 2)} * * where G_n = C(n‑1, 3) · 2^{C(n‑1, 2)}
 *            is the number of all graphs * (not necessar...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static size_t memo_cap = 0;

static seq_t *pow2C = NULL;
static size_t pow2C_cap = 0;

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    /* Ensure memo table is large enough. */
    if (n + 1 > memo_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failed */
        memo = tmp;
        /* initialise new entries to 0 */
        memset(&memo[memo_cap], 0, (new_cap - memo_cap) * sizeof(seq_t));
        memo_cap = new_cap;
    }

    /* Ensure pow2C table is large enough. */
    if (n > pow2C_cap) {
        size_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(pow2C, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        pow2C = tmp;
        /* compute new entries */
        for (size_t i = pow2C_cap; i < new_cap; ++i) {
            /* exponent = i*(i-1)/2 */
            idx_t exp = (idx_t)((((unsigned __int128)i) * (i - 1)) >> 1);
            pow2C[i] = pow2(exp);
        }
        pow2C_cap = new_cap;
    }

    /* Compute a′_n for all n up to the requested n if not already computed */
    for (idx_t cur = 4; cur <= n; ++cur) {
        if (memo[cur]) continue; /* already computed */

        /* G_cur = C(cur-1,3) * 2^{C(cur-1,2)} */
        seq_t G_cur = binomial(cur - 1, 3);
        G_cur *= pow2C[cur - 1];

        seq_t sum = 0;
        for (idx_t k = 1; k < cur; ++k) {
            if (!memo[k]) continue; /* a′_k = 0 for k<4 */
            seq_t term = memo[k];
            term *= binomial(cur - 1, k - 1);
            term *= pow2C[cur - k];
            sum += term;
        }

        memo[cur] = G_cur - sum;
    }

    /* Result for the original problem: n * a′_n */
    seq_t res = memo[n];
    res *= (seq_t)n;
    return res;
}
