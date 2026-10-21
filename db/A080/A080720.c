/*
 * A080720  a(0) = 5; for n>0, a(n) is taken to be the smallest positive
 *          integer greater than a(n-1) which is consistent with the condition
 *          "n is a member of the sequence if and only if a(n) is a multiple
 *          of 3".
 * https://oeis.org/A080720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 5; For n > 0, a(n) is the smallest integer greater than
 *            a(n-1) such that a(n) is a multiple of 3 iff n appears among the
 *            values {a(0), a(1), …}. The sequence satisfies a(a(n)) =
 *            3*(n+4). Computed by incremental search with memoisation
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *seq      = NULL;
    static unsigned char *used = NULL;
    static idx_t seq_cap   = 0;
    static idx_t used_cap  = 0;
    static idx_t last_computed = 0;
    static seq_t last_value = 0;
    static bool init = false;

    /* --- initialization (first call) ------------------------------------------------- */
    if (!init) {
        /* initial capacities */
        seq_cap  = 16;
        used_cap = 128;

        seq  = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        used = (unsigned char*)malloc(used_cap * sizeof(unsigned char));
        if (!seq || !used) abort();

        /* clear used array */
        memset(used, 0, used_cap * sizeof(unsigned char));

        /* first term */
        seq[0] = (seq_t)5;
        last_value = seq[0];
        used[(idx_t)5] = 1;   /* mark 5 as used */
        last_computed = 0;
        init = true;
    }

    /* --- grow seq array if needed --------------------------------------------- */
    if (n + (idx_t)1 > seq_cap) {
        idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap < n + (idx_t)1) new_cap = n + (idx_t)1;
        seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* --- grow used array at least to indices needed ----------------------------- */
    /* ensure indices up to n are accessible: */
    idx_t min_used = n + (idx_t)1;
    /* also ensure we can index candidate values; a(a(k)) = 3*(k+4) gives rough bound */
    idx_t bound_used = 3 * ((idx_t)n + 4) + 10;
    if (bound_used > min_used) min_used = bound_used;

    if (min_used > used_cap) {
        idx_t new_used = used_cap ? used_cap * 2 : 128;
        if (new_used < min_used) new_used = min_used;
        unsigned char *tmp = (unsigned char*)realloc(used, new_used * sizeof(unsigned char));
        memset(tmp + used_cap, 0, (new_used - used_cap) * sizeof(unsigned char));
        used = tmp;
        used_cap = new_used;
    }

    /* --- compute missing terms up to n (increasing order) ---------------------- */
    for (idx_t i = last_computed + (idx_t)1; i <= n; ++i) {
        /* candidate starts just above the previous term */
        seq_t v = last_value + (seq_t)1;

        /* does i already appear among the values? */
        bool i_is_member = (i < used_cap && used[(idx_t)i]);

        /* search for the smallest suitable value */
        while (1) {
            /* ensure used array covers v */
            if ((idx_t)v >= used_cap) {
                idx_t new_used = used_cap ? used_cap * 2 : 128;
                if (new_used < (idx_t)v + (idx_t)1) new_used = (idx_t)v + (idx_t)1;
                unsigned char *tmp = (unsigned char*)realloc(used, new_used * sizeof(unsigned char));
                memset(tmp + used_cap, 0, (new_used - used_cap) * sizeof(unsigned char));
                used = tmp;
                used_cap = new_used;
            }

            if (used[(idx_t)v]) {   /* already used – skip */
                ++v; continue;
            }

            if (i_is_member) {      /* must be multiple of 3 */
                if ((v % (seq_t)3) == (seq_t)0) break;
            } else {                /* must not be multiple of 3 */
                if ((v % (seq_t)3) != (seq_t)0) break;
            }
            ++v;
        }

        seq[i] = v;
        used[(idx_t)v] = 1;
        last_value = v;
    }

    last_computed = n;
    return seq[n];
}
