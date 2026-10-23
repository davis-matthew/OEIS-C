/*
 * A024383  a(n) = s(1)*s(2)*...*s(n)*(1/s(1) - 1/s(2) + ... + c/s(n)), where
 *          c = (-1)^(n+1) and s(k) = 4*k - 3 for k = 1, 2, 3, ....
 * https://oeis.org/A024383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + (4*n-3)^2 * a(n-2) with a(1) = 1, a(2) = 4
 *            (offset 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for the recurrence values */
static seq_t *rec = NULL;      /* array of seq_t holding a(1)..a(n)   */
static idx_t rec_size = 0;     /* number of valid entries in rec (rec_size >= 1) */
static idx_t rec_capacity = 0; /* allocated capacity of rec */

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* the problem never calls with n == 0; if it does we simply use 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* initialise the cache on first use */
    if (rec == NULL) {
        /* allocate an initial small capacity */
        rec_capacity = 4;
        rec = (seq_t *)malloc(rec_capacity * sizeof(seq_t));
        if (!rec) abort();          /* allocation failed */

        /* base values for the recurrence (offset 1) */
        rec[0] = (seq_t)1;          /* a(1) */
        if (rec_capacity < 2) {
            rec_capacity = 2;
            seq_t *tmp = (seq_t *)realloc(rec, rec_capacity * sizeof(seq_t));
            if (!tmp) abort();
            rec = tmp;
        }
        rec[1] = (seq_t)4;          /* a(2) */
        rec_size = 2;
    }

    /* grow the cache until it contains a(n) (i.e. index n-1) */
    while (rec_size <= n - 1) {
        /* ensure there is enough space */
        if (rec_size == rec_capacity) {
            idx_t new_cap = rec_capacity * 2;
            seq_t *tmp = (seq_t *)realloc(rec, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            rec = tmp;
            rec_capacity = new_cap;
        }

        /* compute the next recurrence term */
        idx_t i = rec_size;               /* next index to compute (0‑based) */
        seq_t k = (seq_t)((idx_t)4 * i - (idx_t)3);  /* 4*i-3          */
        seq_t factor = k * k;               /* (4*i-3)^2       */
        seq_t term1 = (seq_t)4 * rec[i - 1];
        seq_t term2 = factor * rec[i - 2];
        rec[i] = term1 + term2;

        rec_size++;                        /* new value added */
    }

    /* return the desired term (rec[0] == a(1)) */
    return rec[n - 1];
}
