/*
 * A171889  a(n) = number immediately following n-th 0 in A181391.
 * https://oeis.org/A171889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 9686, u32 n <= 9686, u64 n <= 9686,
 *            u128 n <= 9686, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): the number immediately following the n‑th zero of van
   Eck's sequence (A181391).  The definition follows the rule for
   the van Eck sequence:
   a(1)=0; for k>1, if the previous term has not appeared
   before then a(k)=0, otherwise a(k) = (k-1) - (last index where
   that value appeared).  This function walks the sequence until the
   n‑th zero occurs and returns the value that immediately follows
   that zero. */

#include <stdlib.h>
#include <string.h>

/*  seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    /* Array of last occurrences for each seen value.  The array
       is indexed by the value itself; values are small for
       reasonable n (<=10 000), so a cast to size_t is safe. */
    idx_t *last = NULL;      /* last index where each value appeared */
    size_t last_cap = 0;     /* current allocated capacity */

    seq_t cur = 0;           /* current van Eck term (a(k)) */
    idx_t idx = 1;           /* 1‑based position in the sequence */
    idx_t zeros_seen = 0;    /* how many zeros have appeared so far */

    while (1) {
        /* If this term is 0, check whether it is the n‑th zero. */
        if (cur == 0) {
            zeros_seen++;
            if (zeros_seen == n) {
                /* The following term is determined by the last
                   occurrence of 0, which resides in last[0] before
                   we update it. */
                seq_t next;
                if (last_cap == 0 || last[0] == 0)
                    next = 0;
                else
                    next = (seq_t)(idx - last[0]);

                free(last);
                return next;
            }
        }

        /* Ensure the array can index the current value. */
        size_t val = (size_t)cur;
        if (val >= last_cap) {
            size_t new_cap = last_cap ? last_cap : 1;
            while (val >= new_cap)
                new_cap <<= 1;           /* double until it fits */
            idx_t *tmp = (idx_t *)realloc(last,
                                          new_cap * sizeof(idx_t));
            if (!tmp) {
                free(last);
                abort();                /* allocation failure */
            }
            /* Zero‑initialize the newly added portion. */
            memset(tmp + last_cap, 0,
                   (new_cap - last_cap) * sizeof(idx_t));
            last = tmp;
            last_cap = new_cap;
        }

        /* Compute the next van Eck term. */
        seq_t next;
        if (last[val] == 0)
            next = 0;
        else
            next = (seq_t)(idx - last[val]);

        /* Record the current value's occurrence. */
        last[val] = idx;

        /* Advance to the next index. */
        idx++;
        cur = next;
    }
}
