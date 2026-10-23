/*
 * A126681  a(n) = Product_{i=5..n} Stirling_2(i,5).
 * https://oeis.org/A126681
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=5..n} Stirling_2(i,5)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Stirling numbers of the second kind for fixed k=5 and builds
   the product sequence a(n).  The function works for any build using
   unsigned integer types or the arbitrary-precision class seq_t.  */
#include <stdlib.h>
#include <string.h>

/* Sequentially compute a(n).  The function is thread‑unsafe (no
   synchronization), but meets the requirements. */
seq_t a(idx_t n)
{
    /* Local static state for memoization */
    static idx_t cur_n           = 0;             /* last n computed     */
    static seq_t s[6] = {0};                    /* S(n,0..5)           */
    static seq_t *prod           = NULL;          /* product array       */
    static size_t prod_len       = 0, prod_cap   = 0;  /* array capacity   */

    /* One‑time initialization */
    if (!prod) {
        /* S(0,0)=1, others 0 */
        s[0] = (seq_t)1;
        for (idx_t k = 1; k <= 5; ++k) s[k] = (seq_t)0;
    }

    /* Never asked for indices below offset; no meaningful value needed */
    if (n < 5) return (seq_t)1;

    /* Extend precomputed values up to requested n */
    while (cur_n < n) {
        ++cur_n;
        /* Update Stirling numbers via recurrence:
           S(n,k) = k*S(n-1,k) + S(n-1,k-1)   (k >= 1)    */
        for (idx_t k = 5; k >= 1; --k) {
            s[k] = s[k] * (seq_t)k + s[k - 1];
        }
        s[0] = (seq_t)0;  /* S(n,0) = 0 for n >= 1 */

        if (cur_n >= 5) {
            seq_t newProd;
            if (prod_len == 0) {
                /* First product term: a(5) = S(5,5) = 1 */
                newProd = s[5];
            } else {
                newProd = prod[prod_len - 1] * s[5];
            }

            /* Grow product array if needed */
            if (prod_len == prod_cap) {
                size_t new_cap = prod_cap ? prod_cap * 2 : 4;
                seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (prod) {
                    memcpy(newp, prod, prod_len * sizeof(seq_t));
                    free(prod);
                }
                prod = newp;
                prod_cap = new_cap;
            }

            prod[prod_len++] = newProd;
        }
    }

    /* Retrieve a(n) from memoized array */
    return prod[n - 5];
}
