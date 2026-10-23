/*
 * A151683  Irregular triangle read by rows: row n (n>=0) gives
 *          binomial(wt(n+k),k), k >= 0, up to the point where the terms are
 *          all zeros (wt() = A000120()).
 * https://oeis.org/A151683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th term of the flattened irregular triangle
 *            defined by T(n,k) = binomial(popcount(n+k),k) for k ≥ 0. The
 *            sequence is obtained by concatenating the rows of this triangle
 *            row by row, where row n consists of the values of k such that k
 *            ≤ popcount(n+k)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple on‑demand generator and memoizes all
   terms already produced, so repeated calls in any order are
   efficient.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* multiplicative binomial coefficient: based on small n and k
   (k ≤ n).  Works for any seq_t that supports *, / and integer
   conversions. */
static seq_t
binom_small(unsigned n, unsigned k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (unsigned i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t
a(idx_t n)
{
    /* memoised table of generated terms */
    static seq_t *table     = NULL;
    static size_t table_len = 0;
    static size_t table_cap = 0;

    /* next row that has not been processed yet */
    static idx_t next_row = 0;

    /* generate rows until the table is long enough */
    while (table_len <= (size_t)n) {
        /* process row 'next_row' */
        for (idx_t k = 0; ; ++k) {
            idx_t m   = next_row + k;
            unsigned pop = __builtin_popcountll((unsigned long long)m);
            if ((unsigned)k > pop) break;   /* all subsequent terms are zero */

            seq_t val = binom_small(pop, (unsigned)k);

            /* enlarge buffer if necessary */
            if (table_len == table_cap) {
                size_t new_cap = table_cap ? table_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* allocation failure – abort */
                table     = tmp;
                table_cap = new_cap;
            }
            table[table_len++] = val;
        }
        ++next_row;   /* all terms of this row processed */
    }
    return table[(size_t)n];
}
