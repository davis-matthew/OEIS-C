/*
 * A206560  Number of 10's in the last section of the set of partitions of n.
 * https://oeis.org/A206560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let p(k) be the partition number A000041(k). Then for k >= 0:
 *            p(k) = sum_{j=1..10} a(k+j) Consequently, for n >= 11: a(n) =
 *            a(n-10) + p(n-10) - p(n-11) Base values (OEIS): a(1) = … = a(9)
 *            = 0, a(10) = 1
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


static seq_t *p_arr = NULL;     /* p[n] — partition numbers */
static seq_t *a_arr = NULL;     /* a[n] — sequence values   */
static size_t  cap   = 0;       /* allocated capacity  */
static idx_t   last_n= 0;       /* last index with data   */

/* Allocate or enlarge the arrays so that indices up to n exist.  */
static void ensure_capacity(idx_t n)
{
    if (!p_arr) {
        size_t init_cap = (size_t)(n < 64 ? 64 : n * 2);
        p_arr = (seq_t *)calloc(init_cap, sizeof(seq_t));
        a_arr = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (!p_arr || !a_arr) abort();

        /* base cases */
        p_arr[0] = (seq_t)1;            /* p(0) = 1 */
        for (idx_t i = 1; i < 11 && i < init_cap; ++i) {
            a_arr[i] = (seq_t)0;          /* a(1)..a(9) = 0 */
        }
        if (init_cap > 10) a_arr[10] = (seq_t)1;   /* a(10) = 1 */

        cap   = init_cap;
        last_n = 0;
        return;
    }

    if (cap <= (size_t)n) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        p_arr = (seq_t *)realloc(p_arr, new_cap * sizeof(seq_t));
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (!p_arr || !a_arr) abort();

        /* zero the new part */
        for (idx_t i = cap; i <= n; ++i) {
            p_arr[i] = (seq_t)0;
            a_arr[i] = (seq_t)0;
        }
        cap = new_cap;
    }
}

/* Compute partition numbers p(k) for k from last_n+1 to n using Euler's pentagonal
   recurrence.  It accumulates positive and negative contributions separately to
   avoid underflow for unsigned seq_t types. */
static void compute_partitions(idx_t n)
{
    for (idx_t m = last_n + 1; m <= n; ++m) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t g = k * (3 * k - 1) / 2;
            if (g > m) break;
            if (k & 1)
                pos += p_arr[m - g];
            else
                neg += p_arr[m - g];

            idx_t h = k * (3 * k + 1) / 2;
            if (h > m) break;               /* h > m implies all later g/h > m */
            if (k & 1)
                pos += p_arr[m - h];
            else
                neg += p_arr[m - h];
        }
        /* p(m) = pos - neg  (pos >= neg for all m) */
        p_arr[m] = pos - neg;
    }
    last_n = n;
}

/* Compute a(n) values for n > current last stored index.  */
static void compute_a(idx_t n)
{
    for (idx_t i = a_arr ? 11 : 1; i <= n; ++i) {
        if (i <= 10) continue;  /* base terms already set */
        /* a(i) = a(i-10) + p(i-10) - p(i-11) */
        a_arr[i] = a_arr[i - 10] + p_arr[i - 10] - p_arr[i - 11];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but safe */

    /* Ensure arrays are large enough and up to date */
    ensure_capacity(n);
    if (n > last_n) {
        compute_partitions(n);
        compute_a(n);
    }
    return a_arr[n];
}
