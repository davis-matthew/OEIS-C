/*
 * A179079  Permanent of the n X n matrix whose (i,j)-element is (i+j-1)
 *          modulo 3.
 * https://oeis.org/A179079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the n×n matrix with (i+j-1) mod 3
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- cached factorials ---------- */
static seq_t *fact_cache = NULL;   /* factorials 0! … m!   */
static idx_t  fact_cap = 0;        /* largest m already cached */

static void ensure_factorial(idx_t m)
{
    if (m <= fact_cap) return;

    /* extend the cache to hold up to m */
    seq_t *tmp = (seq_t *)realloc(fact_cache, (m + 1) * sizeof(seq_t));
    if (!tmp) exit(1);
    fact_cache = tmp;

    /* initialise the empty cache if needed */
    if (fact_cap == 0) {
        fact_cache[0] = 1;
        fact_cap = 0;
    }

    /* compute new factorial values */
    for (idx_t i = fact_cap + 1; i <= m; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_cap = m;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* row/column residue counts (0,1,2) */
    idx_t m0 = n / 3;                     /* residue 0 */
    idx_t m1 = m0 + (n % 3 >= 1 ? 1 : 0); /* residue 1 */
    idx_t m2 = m0 + (n % 3 == 2 ? 1 : 0); /* residue 2 */

    /* factorials up to the largest of m0,m1,m2 */
    idx_t max_m = m0;
    if (m1 > max_m) max_m = m1;
    if (m2 > max_m) max_m = m2;
    ensure_factorial(max_m);

    /* bounds for the free parameter a */
    idx_t diff  = m0 + m1 - m2;           /* always non‑negative */
    idx_t U = (diff < m0) ? diff : m0;   /* min(m0, m0+m1-m2)   */
    idx_t diff2 = m0 > m2 ? m0 - m2 : 0;
    idx_t L = diff2;                      /* max(0, m0-m2)       */

    seq_t total = 0;

    for (idx_t a_val = L; a_val <= U; ++a_val) {
        /* derived counts */
        idx_t b = m0 + m1 - m2 - a_val;   /* x11   */
        idx_t c = m0 - a_val;             /* x20   */

        /* exponent of 2 in this term */
        idx_t k = 3 * a_val + 2 * (m2 - m0);

        /* build the term multiplicatively, dividing as soon as possible */
        seq_t term = 1;

        /* row 0 part  (residue 0) */
        term *= fact_cache[m0];
        term /= fact_cache[a_val];
        term *= fact_cache[m0];
        term /= fact_cache[m0 - a_val];

        /* row 1 part  (residue 1) */
        term *= fact_cache[m1];
        term /= fact_cache[b];
        term *= fact_cache[m1];
        term /= fact_cache[m1 - b];

        /* row 2 part  (residue 2) */
        term *= fact_cache[m2];
        term /= fact_cache[c];
        term *= fact_cache[m2];
        term /= fact_cache[m2 - c];

        /* multiply by 2^k (k is small for all n whose value fits) */
        term <<= k;          /* safe for k ≤ 63 in the 64‑bit build      */
                              /* for arbitrary‑precision seq_t it is the usual big‑int shift */

        total += term;
    }
    return total;
}
