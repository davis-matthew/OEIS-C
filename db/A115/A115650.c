/*
 * A115650  Triangular numbers which are the sum of distinct double factorials
 *          (A006882).
 * https://oeis.org/A115650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th triangular number that can be expressed
 * as a sum of distinct double factorials (A006882).  The algorithm
 * generates triangular numbers in ascending order, keeps a list of
 * all double factorials up to the current triangular number, and
 * tests each triangular number for a subset‑sum representation.
 * This is a search rather than a closed formula.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* Helper: compute n!!  (n≥0) */
static seq_t double_factorial(int n)
{
    seq_t val = 1;
    for (int k = n; k >= 1; k -= 2)
        val *= (seq_t)k;
    return val;
}

/* --------------------------------------------------------------------- */
/* Recursive subset‑sum test: can target be formed from the first `used`
 * elements of `doubles` (all distinct)?  Uses simple back‑tracking;
 * `used` is the number of elements considered (i.e. doubles[0..used-1]). */
static bool dfs_sum(const seq_t *doubles, size_t used, seq_t target)
{
    if (target == (seq_t)0)
        return true;
    if (used == (size_t)0)
        return false;

    seq_t v = doubles[used - 1];
    if (v > target)
        return dfs_sum(doubles, used - 1, target);
    if (v == target)
        return true;

    /* try using v */
    if (dfs_sum(doubles, used - 1, target - v))
        return true;
    /* or skip v */
    return dfs_sum(doubles, used - 1, target);
}

/* --------------------------------------------------------------------- */
/* Global data: list of distinct double factorials (1 is stored once)   */
static seq_t *doubles = NULL;      /* pointer to array of double factorials */
static size_t  d_len   = 0;         /* number of stored double factorials   */
static int     next_n  = 2;         /* next integer to compute n!!         */

/* Extend the list of double factorials so that all double factorials
 * <= limit are present.  The list grows only when a new triangular
 * number exceeds the previous maximum. */
static void ensure_doubles_up_to(seq_t limit)
{
    /* initialise with a single 1 (0!! = 1 = 1!!) */
    if (d_len == 0) {
        doubles = (seq_t *)malloc(sizeof(seq_t));
        if (!doubles) return; /* allocation failure – silently abort */
        doubles[0] = (seq_t)1;
        d_len = 1;
    }

    while (1) {
        seq_t val = double_factorial(next_n);
        if (val > limit)
            break;
        seq_t *tmp = (seq_t *)realloc(doubles, (d_len + 1) * sizeof(seq_t));
        if (!tmp) return; /* allocation failure – silently abort */
        doubles = tmp;
        doubles[d_len] = val;
        ++d_len;
        ++next_n;
    }
}

/* --------------------------------------------------------------------- */
/* Public sequence function: a(idx_t n)                                 */
seq_t a(idx_t n)
{
    static seq_t *terms   = NULL; /* cache of found triangular numbers   */
    static idx_t  terms_n = 0;    /* current number of stored terms      */
    static idx_t  last_t  = 1;    /* last triangular index processed     */

    /* If we already have the requested term, return it. */
    if (terms_n >= n)
        return terms[n - 1];

    /* initialise the terms array once */
    if (!terms) {
        terms = (seq_t *)malloc(32 * sizeof(seq_t));
        if (!terms) return (seq_t)0; /* allocation failure – return 0   */
    }
    size_t terms_cap = 32;

    /* Generate triangular numbers until we have n terms. */
    while (terms_n < n) {
        seq_t T = (seq_t)last_t * ((seq_t)last_t + (seq_t)1) / (seq_t)2;

        /* make sure all double factorials <= T are available */
        ensure_doubles_up_to(T);

        /* count how many double factorials are ≤ T */
        size_t used = 0;
        while (used < d_len && doubles[used] <= T)
            ++used;

        /* test representability */
        if (dfs_sum(doubles, used, T)) {
            /* grow terms array if needed */
            if (terms_n == terms_cap) {
                terms_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure – return 0   */
                terms = tmp;
            }
            terms[terms_n++] = T;
        }
        ++last_t; /* advance to next triangular index */
    }

    return terms[n - 1];
}
