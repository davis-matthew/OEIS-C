/*
 * A116221  If X_1,...,X_n is a partition of a 5n-set X into 5-blocks then
 *          a(n) is equal to the number of permutations f of X such that
 *          f(X_i) <> X_i, (i=1,...,n).
 * https://oeis.org/A116221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (-120)^j*binomial(n,j)*(5*n-5*j)!
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Memoised factorial table */
static seq_t *fact_tbl = NULL;
static size_t fact_cap = 0;

/* Ensure factorials up to 5*n are cached */
static void ensure_factorials(idx_t n)
{
    /* factorials from 0! up to (5*n)! */
    size_t need = 5 * n + 1;
    if (need <= fact_cap) return;

    seq_t *new_tbl = (seq_t *)realloc(fact_tbl, need * sizeof(seq_t));
    if (!new_tbl) exit(EXIT_FAILURE);          /* memory exhausted */
    size_t start = fact_cap;
    if (fact_cap == 0) {
        /* initialise 0! = 1 */
        new_tbl[0] = seq_mul((seq_t)1, (seq_t)1);
        start = 1;
    }
    for (size_t i = start; i < need; ++i) {
        new_tbl[i] = seq_mul(new_tbl[i-1], (seq_t)i);
    }
    fact_tbl = new_tbl;
    fact_cap = need;
}

seq_t a(idx_t n)
{
    if (n == 0) return seq_mul((seq_t)0, (seq_t)1); /* undefined, but returns 0 */

    ensure_factorials(n);

    seq_t result = (seq_t)0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t bin = binomial(n, j);
        seq_t fact_term = fact_tbl[5 * (n - j)];
        seq_t pow_val  = seq_pow((seq_t)120, j);
        seq_t term = seq_mul(seq_mul(bin, fact_term), pow_val);

        if (j & 1) {
            result -= term;
        } else {
            result += term;
        }
    }
    return result;
}
