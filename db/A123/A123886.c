/*
 * A123886  a(0)=1. a(n) = a(n-1) + (number of earlier terms {i.e., terms a(0)
 *          through a(n-1)} that divide n).
 * https://oeis.org/A123886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + (# of earlier terms that divide n), a(0)=1
 * Range:     u8 n <= 89, u16 n <= 8748, u32 n <= 8748, u64 n <= 8748,
 *            u128 n <= 8748, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external linkage required */
seq_t a(idx_t n)
{
    /* static state: sequence values and how far we have computed */
    static seq_t *seq = NULL;      /* array of a(i) */
    static size_t seq_cap = 0;     /* allocated capacity of seq */
    static idx_t  computed = 0;    /* number of terms computed so far */

    /* helper to ensure capacity for indices up to new_n */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed – abort silently */
        seq = tmp;
        seq_cap = new_cap;
    }

    /* if nothing has been computed yet, initialise a(0) */
    if (computed == 0) {
        seq[0] = (seq_t)1;
        computed = 1;
    }

    /* compute missing terms up to n */
    for (idx_t idx = computed; idx <= n; ++idx) {
        if (idx == 0) {          /* a(0) already known */
            seq[0] = (seq_t)1;
            continue;
        }

        seq_t count = 0;         /* number of earlier terms that divide idx */

        /* examine all previous a(j) */
        for (idx_t j = 0; j < idx; ++j) {
            seq_t v = seq[j];
            /* v divides idx only if v <= idx */
            if (v <= idx) {
                if (idx % (idx_t)v == 0)  /* safe cast: v <= idx <= idx_t max */
                    ++count;
            }
        }

        seq[idx] = seq[idx - 1] + count;
    }

    computed = n + 1;   /* we now have values up to a(n) */
    return seq[n];
}
