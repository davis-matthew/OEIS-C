/*
 * A109593  Sequence and first differences include all odd numbers exactly
 *          once.
 * https://oeis.org/A109593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=1, a(2)=4, a(3)=9, a(4)=16; For n>4: a(n)=a(n-1)+d where d
 *            is the smallest odd integer that has not been used as a previous
 *            term or as a previous first difference
 * Range:     u8 n <= 15, u16 n <= 249, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#define USED_SIZE 65536   /* enough for all odd candidates up to n≈10 000 */

seq_t a(idx_t n)
{
    /* Static state for memoisation and odd‑number bookkeeping */
    static bool initialized = false;
    static seq_t *values   = NULL;   /* array of computed terms (0‑based) */
    static idx_t  capacity = 0;      /* current allocated size of values[] */
    static idx_t  last     = 0;      /* number of terms computed (>=4) */
    static uint8_t used[USED_SIZE] = {0}; /* 1 → odd number already used */
    static idx_t  diff_candidate = 1;     /* next odd to try as a difference */

    if (!initialized) {
        /* initial terms */
        capacity = 64;
        values   = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!values) return (seq_t)0;   /* allocation failure */
        values[0] = (seq_t)1;   /* a(1) */
        values[1] = (seq_t)4;   /* a(2) */
        values[2] = (seq_t)9;   /* a(3) */
        values[3] = (seq_t)16;  /* a(4) */
        last = 4;

        /* mark used odd numbers (terms and first differences) */
        used[1] = 1; /* term a(1)    */
        used[3] = 1; /* diff a(2)-a(1) */
        used[5] = 1; /* diff a(3)-a(2) */
        used[7] = 1; /* diff a(4)-a(3) */
        used[9] = 1; /* term a(3)      */

        initialized = true;
    }

    if (n == 0) return (seq_t)0;          /* undefined, safety */

    if (n <= last) return values[n-1];     /* already computed */

    /* extend the array up to term n */
    for (idx_t k = last + 1; k <= n; ++k) {
        /* find smallest unused odd that can serve as the next difference */
        idx_t cand = diff_candidate;
        while (cand < USED_SIZE && used[cand]) cand += 2;
        diff_candidate = cand;              /* next search starts here */

        /* compute new term */
        seq_t prev = values[last-1];
        seq_t new_val = prev + (seq_t)cand;

        /* mark used odd numbers */
        if (cand < USED_SIZE) used[(size_t)cand] = 1;
        if ((new_val & (seq_t)1) && /* odd? */
            new_val <= (seq_t)(USED_SIZE - 1)) {
            used[(size_t)new_val] = 1;
        }

        /* ensure enough capacity for the new entry */
        if (last == capacity) {
            capacity *= 2;
            seq_t *tmp = (seq_t *)realloc(values, capacity * sizeof(seq_t));
            if (!tmp) {
                free(values);
                return (seq_t)0;            /* allocation failure */
            }
            values = tmp;
        }

        values[last] = new_val;
        last = k;        /* increment count of computed terms */
    }

    return values[n-1];
}
