/*
 * A058808  Product{k=1 to n}[S(n,k)], where S(n,k) is a Stirling number of
 *          the second kind. (S(n,k) = number of ways of partitioning a set of
 *          n elements into k nonempty subsets.)
 * https://oeis.org/A058808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A058808 – Product of Stirling numbers of the second kind
   a(n) = ∏_{k=1}^{n} S(n,k)
   S(n,k) = S(n‑1,k‑1) + k * S(n‑1,k),  S(1,1)=1 */

/* The routine keeps the current row of Stirling numbers in place.
   The row is extended as needed and the products a(i) for all i
   already computed are cached in a separate array.  Calls may arrive
   in any order; the function will extend the pre‑computed range
   up to the requested n. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* out of range */

    /* static state shared across calls */
    static seq_t *row      = NULL;      /* S(cur,k) for k=0..cur */
    static seq_t *prod     = NULL;      /* a(i) for i<=computed */
    static size_t row_cap  = 0;
    static size_t prod_cap = 0;
    static idx_t  computed = 0;         /* largest n already computed */

    /* first call – initialise for n = 1 */
    if (computed == 0) {
        row_cap  = 2;          /* need indices 0..1               */
        prod_cap = 2;
        row  = (seq_t*)malloc(row_cap * sizeof(seq_t));
        prod = (seq_t*)malloc(prod_cap * sizeof(seq_t));
        if (!row || !prod) return 0;         /* allocation failure */

        row[0] = 0;    /* unused, keep zero */
        row[1] = 1;    /* S(1,1) = 1 */
        prod[1] = 1;   /* a(1) = 1 */
        computed = 1;
    }

    /* extend computation up to requested n */
    for (idx_t cur = computed + 1; cur <= n; ++cur) {

        /* extend row array if necessary (size cur+1 needed) */
        if (row_cap < (size_t)(cur + 1)) {
            size_t new_cap = (size_t)(cur + 1);
            seq_t *tmp = (seq_t*)realloc(row, new_cap * sizeof(seq_t));
            if (!tmp) return 0;               /* allocation failure */
            row = tmp;
            row_cap = new_cap;
        }

        /* extend prod array if necessary */
        if (prod_cap < (size_t)(cur + 1)) {
            size_t new_cap = (size_t)(cur + 1);
            seq_t *tmp = (seq_t*)realloc(prod, new_cap * sizeof(seq_t));
            if (!tmp) return 0;               /* allocation failure */
            prod = tmp;
            prod_cap = new_cap;
        }

        /* compute new Stirling row in place, descending k */
        row[cur] = 1;                         /* S(cur,cur) = 1 */
        for (idx_t k = cur - 1; k >= 1; --k) {
            row[k] = row[k - 1] + k * row[k];
        }

        /* compute product a(cur) = ∏_{k=1}^{cur} row[k] */
        seq_t prod_cur = 1;
        for (idx_t k = 1; k <= cur; ++k) {
            prod_cur *= row[k];
        }
        prod[cur] = prod_cur;
    }

    computed = n;      /* now we have values up to n */
    return prod[n];
}
