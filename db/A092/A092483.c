/*
 * A092483  a(n) is the floor of the average of the 1st moment of all previous
 *          entries.
 * https://oeis.org/A092483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092483
   a(n) = floor( sum_{j=1}^{n-1} j*a(j) / (n-1) ),   a(1) = 1   */

/* Note: idx_t is the index type (uint64_t) and seq_t is the
   value type (uint8_t … uint64_t or arbitrary‑precision). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state --------------------------------------------------- */
    static seq_t *seq = NULL;          /* cache a[1..max_n]           */
    static idx_t  max_n = 0;           /* allocated size of seq array */
    static seq_t  sum = 0;             /* sum_{j=1}^{cur_n} j*a(j)   */
    static idx_t  last = 0;            /* highest n for which a(n) computed */

    /* first call – allocate and initialise a(1) --------------------- */
    if (seq == NULL) {
        /* Allocate room for indices 0..2 (use index 0 as unused) */
        seq = (seq_t *)malloc(sizeof(seq_t) * 3);
        if (!seq) return (seq_t)0;          /* allocation failure – unlikely */
        max_n = 3;                          /* storage for indices 0,1,2 */
        seq[1] = (seq_t)1;                  /* a(1) is fixed to 1 */
        sum   = (seq_t)1;                   /* sum_{j=1}^{1} j*a(j) = 1*1 */
        last  = 1;
    }

    /* compute up to the requested n (if needed) ---------------------- */
    if (n > last) {
        /* ensure we have enough capacity for index n */
        if (n + 2 > max_n) {               /* extra slots to avoid frequent reallocs */
            idx_t new_cap = max_n * 2;
            if (new_cap < n + 2) new_cap = n + 2;
            seq = (seq_t *)realloc(seq, sizeof(seq_t) * new_cap);
            if (!seq) return (seq_t)0;      /* allocation failure */
            max_n = new_cap;
        }

        /* fill in missing terms sequentially */
        for (idx_t m = last + 1; m <= n; ++m) {
            /* a(m) = floor( sum_{j=1}^{m-1} j*a(j) / (m-1) ) */
            seq_t next = sum / (seq_t)(m - 1);
            seq[m] = next;

            /* update cumulative sum for the next iteration:
               sum_{j=1}^{m} j*a(j) = previous_sum + m*a(m) */
            sum += (seq_t)m * next;
        }
        last = n;
    }

    /* return the required term --------------------------------------- */
    return seq[n];
}
