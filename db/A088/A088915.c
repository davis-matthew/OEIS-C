/*
 * A088915  Nonnegative numbers of the form mn(m+n) with integers m,n.
 * https://oeis.org/A088915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = non‑negative integers that can be expressed as * m * n *
 *            (m + n) for some integers m, n. * a(1) = 0; the sequence is the
 *            sorted list of distinct * such values
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---\\ static storage --------------------------------------------- */
static seq_t *a_vals = NULL;   /* array of computed terms                */
static size_t a_count = 0;     /* number of terms currently stored      */
static seq_t  a_limit = 0;     /* current maximal value used in build   */

/* ---\\ helper comparison for qsort -------------------------------- */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* ---\\ rebuild the table for a given maximal value ---------------- */
static void rebuild(seq_t limit)
{
    size_t cap   = 200000;          /* initial capacity                   */
    size_t i, j, m, n;
    seq_t val;

    if (a_vals != NULL) {
        free(a_vals);
    }

    a_vals  = (seq_t *)malloc(cap * sizeof(seq_t));
    if (a_vals == NULL) {
        /* out of memory – terminate the program */
        exit(1);
    }
    a_count = 1;             /* start with a(1)=0                     */
    a_vals[0] = 0;

    for (m = 1; ; ++m) {
        seq_t mm   = (seq_t)m * (seq_t)m;
        seq_t minv = mm * (seq_t)(2 * m);
        if (minv > limit) break;

        for (n = m; ; ++n) {
            val = (seq_t)m * n * (seq_t)(m + n);
            if (val > limit) break;

            if (a_count == cap) {              /* grow buffer */
                cap <<= 1;
                a_vals = (seq_t *)realloc(a_vals, cap * sizeof(seq_t));
                if (a_vals == NULL) {          /* out of memory */
                    exit(1);
                }
            }
            a_vals[a_count++] = val;
        }
    }

    /* sort and unique */
    qsort(a_vals, a_count, sizeof(seq_t), cmp_seq);
    for (i = 1, j = 1; j < a_count; ++j) {
        if (a_vals[j] != a_vals[i - 1]) {
            a_vals[i++] = a_vals[j];
        }
    }
    a_count = i;

    a_limit = limit;            /* remember the limit used */
}

/* ---\\ public interface -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;                 /* undefined, but return 0 */
    }

    /* initialise or enlarge table if needed */
    if (a_vals == NULL) {
        rebuild((seq_t)2500000);   /* initial guess */
    } else if ((size_t)n > a_count) {
        /* grow limit until we have enough terms */
        seq_t new_limit = a_limit;
        while ((size_t)n > a_count) {
            new_limit *= 2;
            rebuild(new_limit);
        }
    }

    return a_vals[n - 1];        /* a(1) is stored at index 0 */
}
