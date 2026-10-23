/*
 * A121639  Number of 2-cell columns in all deco polyominoes of height n. A
 *          deco polyomino is a directed column-convex polyomino in which the
 *          height, measured along the diagonal, is attained only in the last
 *          column.
 * https://oeis.org/A121639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*
 *  Formula:  a(1)=0, a(2)=1, a(3)=5,
 *           a(n) = n*a(n-1) + (n-1)! - (n-3)!   for n >= 4
 */

/* factorial cache: fact_cache[i] = i!   (only for i >= 0) */
static seq_t *fact_cache = NULL;
static size_t fact_len = 0;          /* number of factorials stored, i.e. fact_len = max_index+1 */

/* ensure fact_cache contains factorials up to index n (inclusive) */
static void ensure_fact_upto(idx_t n)
{
    if (fact_len <= (size_t)n)
    {
        size_t new_len = n + 1;
        seq_t *new_ptr = (seq_t *)realloc(fact_cache, new_len * sizeof(seq_t));
        if (!new_ptr) abort();
        fact_cache = new_ptr;
        /* compute new entries */
        for (idx_t i = (idx_t)fact_len; i <= n; ++i)
        {
            if (i == 0)
                fact_cache[0] = (seq_t)1;
            else
                fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_len = new_len;
    }
}

/* value cache: a_cache[i] = a(i)   (1 <= i <= a_len) */
static seq_t *a_cache = NULL;
static size_t a_cap = 0;   /* current allocated size of a_cache (number of slots) */
static idx_t   a_len = 0;   /* highest index that has been defined */

/* ensure a_cache has capacity for indices up to n (inclusive) */
static void ensure_a_cap(idx_t n)
{
    if (a_cap < (size_t)n + 1)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_ptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();
        a_cache = new_ptr;
        a_cap = new_cap;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* handle small cases directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)5;

    /* initialise caches if needed */
    if (a_len < 3)
    {
        ensure_a_cap(3);
        a_cache[1] = (seq_t)0;
        a_cache[2] = (seq_t)1;
        a_cache[3] = (seq_t)5;
        a_len   = 3;
    }

    /* compute any missing terms up to n */
    if (n > a_len)
    {
        ensure_a_cap(n);
        /* factorials up to n-1 are needed for the recurrence */
        ensure_fact_upto(n - 1);

        for (idx_t i = a_len + 1; i <= n; ++i)
        {
            seq_t term = (seq_t)i * a_cache[i - 1]   /* n*a(n-1)   */
                        + fact_cache[i - 1]          /* +(n-1)!    */
                        - fact_cache[i - 3];         /* -(n-3)!    */
            a_cache[i] = term;
        }
        a_len = n;
    }

    return a_cache[n];
}
