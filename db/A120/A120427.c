/*
 * A120427  For each y >= 1 there are only finitely many values of x >= 1 such
 *          that x-y and x+y are both squares; list all such pairs (x,y) with
 *          gcd(x,y) = 1 ordered by values of y; sequence gives y values.
 * https://oeis.org/A120427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th even leg of a primitive Pythagorean triple *
 *            (ordered by the value of the leg)
 * Range:     u8 none, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Comparator for sorting an array of seq_t values. */
static int seq_cmp(const void *p, const void *q)
{
    const seq_t *av = (const seq_t *)p;
    const seq_t *bv = (const seq_t *)q;
    if (*av < *bv) return -1;
    if (*av > *bv) return 1;
    return 0;
}

/* Static storage for the generated sequence. */
static seq_t *y_array = NULL;   /* sorted array of even legs          */
static size_t  y_cnt   = 0;     /* number of generated elements       */
static size_t  y_cap   = 0;     /* current capacity of y_array        */
static idx_t   limit_val = 0;   /* current upper bound used for y     */

/* Generate all 2*m*n <= limit that correspond to primitive triples
 * (i.e. m>n, gcd(m,n)=1, m-n odd).  Results are stored in a
 * freshly allocated and sorted array in y_array. */
static void generate_upto(idx_t limit)
{
    /* free any previously allocated array */
    free(y_array);
    y_array = NULL;
    y_cnt = 0;
    y_cap = 0;
    limit_val = limit;

    /* Fast path: if the limit is zero no values are needed. */
    if (limit == 0) return;

    /* temporary dynamic array for collecting values before sorting */
    size_t cap = 1024;
    y_array = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!y_array) exit(EXIT_FAILURE);
    y_cap = cap;

    idx_t max_m = limit / 2;  /* because 2*m*1 <= limit implies m <= limit/2 */

    for (idx_t m = 2; m <= max_m; ++m) {
        for (idx_t n = 1; n < m; ++n) {
            idx_t y = 2 * m * n;
            if (y > limit) break;           /* further n will only increase y */

            /* primitive condition: m-n odd and coprime */
            if (((m - n) & 1U) && gcd((seq_t)m, (seq_t)n) == (seq_t)1) {
                if (y_cnt == y_cap) {        /* grow temporary array if needed */
                    y_cap <<= 1;
                    y_array = (seq_t *)realloc(y_array, y_cap * sizeof(seq_t));
                    if (!y_array) exit(EXIT_FAILURE);
                }
                y_array[y_cnt++] = (seq_t)y;
            }
        }
    }

    /* sort the array to obtain the sequence order */
    qsort(y_array, y_cnt, sizeof(seq_t), seq_cmp);
}

/* The externally visible OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n=0 */

    /* initial run – generate enough terms for a typical n (tens of thousands) */
    if (y_array == NULL) {
        generate_upto(40000);                  /* about 13,000 values – plenty for n≈10 000 */
    }

    /* If the current array is not long enough, double the bound until it is. */
    while (y_cnt < (size_t)n) {
        idx_t new_limit = limit_val * 2U;
        generate_upto(new_limit);
    }

    return y_array[n - 1];
}
