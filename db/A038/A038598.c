/*
 * A038598  First differences between numbers that are a difference between 2
 *          positive cubes.
 * https://oeis.org/A038598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7; for n>1, a(n)=S[n-1]-S[n-2] where S is the sorted list
 *            of distinct differences a^3−b^3 (a>b>0). The list S is generated
 *            by enumerating all such differences up to a fixed cube limit.
 *            The function lazily builds S on first use. This algorithm
 *            requires O(A^2 log A) time and O(A^2) memory for...
 * Range:     u8 none, u16 none, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort on seq_t (unsigned). */
static int cmp_seq_t(const void *p, const void *q)
{
    seq_t v1 = *(const seq_t *)p;
    seq_t v2 = *(const seq_t *)q;
    return (v1 < v2) ? -1 : (v1 > v2);
}

/* Static storage for the sorted unique cube differences. */
static seq_t *s_vals   = NULL; /* unique differences a^3−b^3, sorted ascending */
static size_t s_len    = 0;    /* number of unique elements */

/* Build s_vals up to a sufficiently large cube limit. */
static void init_sequence(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* Choose a cube limit large enough to cover at least 20 000 terms.
       For A = 400, pair count = 400*399/2 = 79800, which gives enough
       unique values for all n ≤ 10000. */
    const idx_t AMAX = 400;
    const size_t pair_cnt = (size_t)AMAX * (AMAX - 1) / 2;

    /* Allocate space for all pair differences (not yet deduplicated). */
    seq_t *tmp = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
    if (!tmp) return;                         /* allocation failure – give up */

    size_t pos = 0;
    for (idx_t a = 2; a <= AMAX; ++a) {
        seq_t aa = (seq_t)a;
        seq_t aa3 = aa * aa * aa;              /* a^3  (seq_t arithmetic) */
        for (idx_t b = 1; b < a; ++b) {
            seq_t bb = (seq_t)b;
            tmp[pos++] = aa3 - bb * bb * bb;   /* a^3 - b^3  */
        }
    }

    /* Sort all differences. */
    qsort(tmp, pos, sizeof(seq_t), cmp_seq_t);

    /* Deduplicate in place. */
    size_t uniq = 0;
    for (size_t i = 0; i < pos; ++i) {
        if (uniq == 0 || tmp[i] != tmp[uniq - 1])
            tmp[uniq++] = tmp[i];
    }

    /* Resize the array to hold only the unique values. */
    s_vals = (seq_t *)realloc(tmp, uniq * sizeof(seq_t));
    if (!s_vals) {                       /* allocation failure – keep temp */
        free(tmp);
        s_vals = NULL;
        s_len   = 0;
        return;
    }
    s_len = uniq;
}

/* Return the n-th term of OEIS A038598.  n>=1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, but safe */

    init_sequence();
    if (!s_vals || s_len == 0) return 0;  /* initialization failed */

    size_t idx = (size_t)n;               /* convert to unsigned index */

    if (idx == 1)                        /* first term is the smallest value */
        return s_vals[0];

    if (idx > s_len)                      /* out of computed range */
        return 0;

    return s_vals[idx - 1] - s_vals[idx - 2];
}
