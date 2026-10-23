/*
 * A158208  Triangle read by rows: T(n,k) = [x^k] P(n), 0<=k<=n, where P(0) =
 *          2 and P(n) = Sum_{k=0..floor((n-1)/2)} binomial(n, k) * (x - 1)^k
 *          + x^n*Sum_{k=0..floor((n-1)/2)} binomial(n, k) * (1/x-1)^k,
 *          otherwise.
 * https://oeis.org/A158208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less
 * Formula:   a(n) is the flattened triangle whose entries are the
 *            coefficients of the polynomials defined by P(0)=2, P(n)=
 *            Σ_{i=0..⌊(n−1)/2⌋} binomial(n,i)*(x−1)^i + x^n *
 *            Σ_{i=0..⌊(n−1)/2⌋} binomial(n,i)*(1/x−1)^i (n≥1). The sequence
 *            begins: 2; 1,1; 1,0,1; -2,3,3; ... Initial values: a(0)=2,
 *            a(1)=1, a(2)=1,...
 * Range:     i8 n <= 35, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   binomial, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/binomial.h"

/* static storage for the flattened sequence */
static seq_t *cache      = NULL;   /* flattened values a(0),a(1),…   */
static size_t cache_cap  = 0;      /* allocated size of cache        */
static size_t cache_len  = 0;      /* number of entries currently stored */

/* last row index that has been computed; (idx_t)-1 means none yet */
static idx_t last_row     = (idx_t)-1;

/* append the coefficients of row r (0‑based) to the cache */
static void compute_row(idx_t r)
{
    if (r == 0) {
        /* P(0)=2 → a(0)=2 */
        cache[cache_len++] = 2;
        return;
    }

    /* floor((n-1)/2) */
    size_t m = (size_t)((r - 1) / 2);

    for (idx_t k = 0; k <= r; ++k) {
        seq_t sum = 0;

        for (size_t i = 0; i <= m; ++i) {
            seq_t bn = binomial(r, i);          /* binom(r,i)       */

            /* first part: binomial(r,i)*(x-1)^i */
            if (k <= (idx_t)i) {
                seq_t bi_k = binomial(i, (idx_t)k);    /* binom(i,k)    */
                seq_t term = bn * bi_k;
                if (((i - (size_t)k) & 1U) != 0U)
                    term = -term;
                sum += term;
            }

            /* second part: x^r * binomial(r,i)*(1/x-1)^i */
            idx_t nk = r - k;
            if (nk <= (idx_t)i) {
                seq_t bi_nk = binomial(i, nk);         /* binom(i,nk)   */
                seq_t term2 = bn * bi_nk;
                if (((i - (size_t)nk) & 1U) != 0U)
                    term2 = -term2;
                sum += term2;
            }
        }

        cache[cache_len++] = sum;
    }
}

/* ensure that the cache contains all terms up to (and including)
   the given flattened index n.  The cache grows only when necessary. */
static void ensure_cache(idx_t n_index)
{
    if (n_index < cache_len) return;              /* already available   */

    /* determine the row that contains index n_index */
    idx_t target_row;
    if (last_row == (idx_t)-1)
        target_row = 0;
    else
        target_row = last_row;
    while (((target_row + 1) * (target_row + 2)) / 2 <= n_index)
        ++target_row;

    /* capacity needed for all rows up to target_row */
    size_t needed = (size_t)((target_row + 1) * (target_row + 2) / 2);
    if (needed > cache_cap)
        grow_seq(&cache, &cache_cap, needed);

    /* compute missing rows */
    idx_t start_row = (last_row == (idx_t)-1) ? 0 : last_row + 1;
    for (idx_t r = start_row; r <= target_row; ++r)
        compute_row(r);

    last_row = target_row;
}

/* public entry: return a(n) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
