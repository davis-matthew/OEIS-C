/*
 * A137285  a(1)=1. a(n+1) = a(n) + (number of terms of this sequence, from
 *          among terms a(1) through a(n), that are >= (1/n)sum{k=1 to n}
 *          a(k)).
 * https://oeis.org/A137285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n+1)=a(n)+(#of i∈[1..n] with a(i)≥S_n/n), where
 *            S_n=∑_{k=1}^n a(k) (implemented with binary search on the
 *            increasing sequence)
 * Range:     u8 n <= 12, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: sequence values, current size, sum of terms */
    static seq_t *aarr = NULL;
    static size_t cap   = 0;           /* allocated capacity */
    static idx_t  computed = 0;        /* number of terms already computed */
    static seq_t sum = 0;              /* sum of first 'computed' terms */

    if (n < 1) return 0; /* undefined for n=0, but safe guard */

    /* Grow the array if needed */
    if (cap == 0) {
        cap = 8;
        aarr = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Compute terms up to required n */
    while (computed < n) {
        if (computed == 0) {
            /* initialise a(1)=1 */
            aarr[0] = (seq_t)1;
            sum = (seq_t)1;
            computed = 1;
            continue;
        }

        /* binary search for first index where a[i]*computed >= sum */
        size_t left  = 0;
        size_t right = (size_t)computed;        /* one past last index */
        seq_t  sum_prev = sum;                  /* S_{computed} */
        while (left < right) {
            size_t mid = (left + right) >> 1;
            seq_t prod = aarr[mid] * (seq_t)computed;
            if (prod >= sum_prev) right = mid;
            else                   left  = mid + 1;
        }
        size_t first = left;                    /* first index with a[i] >= avg */

        /* count of terms >= average */
        size_t count = (first == (size_t)computed) ? 0 : ((size_t)computed - first);

        /* compute next term a(computed+1) */
        seq_t new_val = aarr[computed - 1] + (seq_t)count;

        /* ensure capacity */
        if (cap <= computed) {
            size_t new_cap = cap ? cap << 1 : 8;
            while (new_cap <= computed) new_cap <<= 1;
            aarr = (seq_t *)realloc(aarr, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* store and update sum */
        aarr[computed] = new_val;
        sum += new_val;
        computed++;
    }

    return aarr[n - 1];
}
