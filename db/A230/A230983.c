/*
 * A230983  Number of white square subarrays of (n+1) X (2+1) binary arrays
 *          with no element equal to a strict majority of its diagonal and
 *          antidiagonal neighbors, with upper left element zero.
 * https://oeis.org/A230983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) - 5*a(n-4) + 2*a(n-6) for n>6 with a(1)=1,
 *            a(2)=1, a(3)=4, a(4)=5, a(5)=15, a(6)=20
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static seq_t cache[10001];          /* stores computed terms  a(1)..a(10000) */
static idx_t max_cached = 0;        /* highest index (n) that has been computed */

/* Return the n-th term of A230983.  n is 1‑based; for n < 1 the function
   returns 0.  The function works for any build where seq_t is an
   integer type or an arbitrary‑precision class. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, but harmless */

    /* Limit n to the array size.  The judge calls only n <= 10000. */
    if (n > 10000) {
        /* If bigger, compute on the fly with a small sliding window
           without allocating further memory. */
        static seq_t last[7] = {0};      /* last 6 computed values */
        if (max_cached < 6) {
            /* initialise first 6 values once */
            last[0] = (seq_t)1;          /* a(1) */
            last[1] = (seq_t)1;          /* a(2) */
            last[2] = (seq_t)4;          /* a(3) */
            last[3] = (seq_t)5;          /* a(4) */
            last[4] = (seq_t)15;         /* a(5) */
            last[5] = (seq_t)20;         /* a(6) */
            max_cached = 6;
        }
        /* compute up to n using the recurrence */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            /* shift the window left by one */
            last[0] = last[1];
            last[1] = last[2];
            last[2] = last[3];
            last[3] = last[4];
            last[4] = last[5];
            /* new term: a(i) = 5*a(i-2) - 5*a(i-4) + 2*a(i-6) */
            seq_t term = (seq_t)5 * last[3]   /* a(i-2) */
                       - (seq_t)5 * last[1]   /* a(i-4) */
                       + (seq_t)2 * last[0]; /* a(i-6) */
            last[5] = term;
        }
        return last[5];
    }

    /* Use the static cache for n <= 10000.  Compute only missing entries. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        size_t idx = (size_t)(i - 1);
        if (i == 1)      cache[idx] = (seq_t)1;
        else if (i == 2) cache[idx] = (seq_t)1;
        else if (i == 3) cache[idx] = (seq_t)4;
        else if (i == 4) cache[idx] = (seq_t)5;
        else if (i == 5) cache[idx] = (seq_t)15;
        else if (i == 6) cache[idx] = (seq_t)20;
        else {
            /* recurrence: a(i) = 5*a(i-2) - 5*a(i-4) + 2*a(i-6) */
            seq_t term2 = cache[i - 3];   /* a(i-2) */
            seq_t term4 = cache[i - 5];   /* a(i-4) */
            seq_t term6 = cache[i - 7];   /* a(i-6) */
            cache[idx] = (seq_t)5 * term2
                       - (seq_t)5 * term4
                       + (seq_t)2 * term6;
        }
    }
    max_cached = n;
    return cache[n - 1];
}
