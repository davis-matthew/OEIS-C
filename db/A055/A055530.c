/*
 * A055530  The recurrence b(k) = 10*b(k-1) + k^n with b(0)=0 has b(k)/10^k
 *          converging to a(n)/9^(n+1).
 * https://oeis.org/A055530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = 10 * sum_{k=1}^{n} S(n,k) * k! * 9^{n-k} where S(n,k) are
 *            Stirling numbers of the second kind
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;      /* cached a(n) values */
static idx_t a_cache_size = 0;     /* number of cached entries */

static seq_t *fact_cache = NULL;   /* factorial values k!  */
static idx_t fact_size = 0;

static seq_t *pow9_cache = NULL;   /* powers 9^i      */
static idx_t pow9_size = 0;

static seq_t *prev_row = NULL;     /* S(n-1, k) values */
static idx_t prev_size = 0;

/* Helper to extend the caches up to new_n (inclusive) */
static void extend_cache(idx_t new_n)
{
    /* Ensure factorial and pow9 tables are large enough */
    if (fact_size <= new_n) {
        fact_cache = (seq_t *)realloc(fact_cache, (new_n + 1) * sizeof(seq_t));
        pow9_cache = (seq_t *)realloc(pow9_cache, (new_n + 1) * sizeof(seq_t));
        for (idx_t i = fact_size; i <= new_n; ++i) {
            if (i == 0) {
                fact_cache[i] = (seq_t)1;
                pow9_cache[i] = (seq_t)1;
            } else {
                fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
                pow9_cache[i] = pow9_cache[i - 1]  * (seq_t)9;
            }
        }
        fact_size = new_n + 1;
        pow9_size = new_n + 1;
    }

    /* Extend a_cache */
    if (a_cache_size <= new_n) {
        a_cache = (seq_t *)realloc(a_cache, (new_n + 1) * sizeof(seq_t));
        for (idx_t i = a_cache_size; i <= new_n; ++i)
            a_cache[i] = 0;              /* initialise to zero */
        a_cache_size = new_n + 1;
    }

    /* If we are beyond previously computed n */
    if (prev_size == 0) {
        /* First time: initialise for n = 0 */
        prev_row = (seq_t *)malloc(sizeof(seq_t));   /* size 1 */
        prev_row[0] = (seq_t)1;                      /* S(0,0) = 1 */
        prev_size = 1;
        a_cache[0] = (seq_t)1;                       /* a(0) = 1 */
    }

    /* Compute rows for n = prev_size .. new_n */
    for (idx_t n = prev_size; n <= new_n; ++n) {
        /* Allocate memory for current row (size n+1) */
        seq_t *curr_row = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        curr_row[0] = 0;                            /* S(n,0) = 0 for n>0 */

        /* Compute Stirling numbers for this row */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term = 0;
            if (k < prev_size)            /* prev_row has size prev_size (=n) */
                term = prev_row[k] * (seq_t)k;
            seq_t add = (prev_size > 0) ? prev_row[k - 1] : 0;
            curr_row[k] = term + add;
        }

        /* Compute a(n) using the derived formula */
        seq_t sum = 0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t prod = curr_row[k] * fact_cache[k];
            sum += prod * pow9_cache[n - k];
        }
        a_cache[n] = sum * (seq_t)10;

        /* Free previous row and update */
        free(prev_row);
        prev_row = curr_row;
        prev_size = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n + 1 > a_cache_size)
        extend_cache(n);
    return a_cache[n];
}
