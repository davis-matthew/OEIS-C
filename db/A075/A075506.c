/*
 * A075506  Shifts one place left under 7th-order binomial transform.
 * https://oeis.org/A075506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, eigen
 * Formula:   a(n) = sum_{m=0}^{n} 7^{\,n-m} * S2(n,m) (S2 Stirling numbers of
 *            2nd kind)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static state for memoisation */
static seq_t *a_vals = NULL;          /* array of computed a(n) */
static seq_t *b = NULL;              /* auxiliary array for recurrence */
static size_t a_cap = 0;             /* allocated size of a_vals   */
static size_t b_cap = 0;             /* allocated size of b        */
static idx_t computed = 0;           /* largest n for which aVals is filled */

/* grow an array of seq_t to at least 'need' elements */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap > 0) ? (*cap) * 2 : 1;
    while (new_cap < need) new_cap *= 2;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    /* initialise the new part to zero */
    for (size_t i = *cap; i < new_cap; ++i) (*arr)[i] = (seq_t)0;
    *cap = new_cap;
}

/* compute a(n) by memoised iterative recurrence */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (a_vals == NULL) {
        a_cap = 1;
        a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_vals[0] = (seq_t)1;      /* a(0) = 1 */

        b_cap = 1;
        b = (seq_t *)malloc(b_cap * sizeof(seq_t));
        b[0] = (seq_t)1;          /* b[0] = 1 for n = 0 */

        computed = 0;
    }

    /* if already computed return stored value */
    if (n <= computed) return a_vals[n];

    /* ensure capacity for the requested n */
    grow_array(&a_vals, &a_cap, (size_t)n + 1);
    grow_array(&b, &b_cap, (size_t)n + 1);

    /* iterate from the next missing index up to n */
    for (idx_t m = computed + 1; m <= n; ++m) {
        seq_t sum_new = (seq_t)0;
        /* update array b in reverse order
           b[k] = 7*k*b[k] + b[k-1]   (k from m down to 1)   */
        for (idx_t k = m; k > 0; --k) {
            seq_t tmp     = b[k];                            /* old value */
            seq_t mult    = (seq_t)(7ULL * (unsigned long long)k);
            seq_t newval  = tmp * mult + b[k - 1];
            b[k]          = newval;
            sum_new       += newval;
        }
        b[0] = (seq_t)0;          /* new b[0] is always 0 for n >= 1 */
        a_vals[m] = sum_new;
    }
    computed = n;
    return a_vals[n];
}
