/*
 * A080589  a(n) is smallest nonnegative integer which is consistent with
 *          sequence being monotonically increasing and satisfying a(a(n)) =
 *          5n.
 * https://oeis.org/A080589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080589
   a(n) = smallest nonnegative integer such that a is monotonic
   increasing and satisfies a(a(n)) = 5*n. */

#include <stdlib.h>
#include <stdbool.h>

/* static tables for memoization */
static seq_t *tab_a      = NULL;   /* a(i)  when defined */
static bool   *tab_def    = NULL;   /* tab_def[i] == true iff a(i) already known */
static bool   *tab_req    = NULL;   /* a(i) is forced to be tab_val[i] */
static seq_t *tab_val    = NULL;   /* required value for i when tab_req[i] is true */
static size_t cap        = 0;       /* current table capacity (number of indices)  */
static size_t computed   = 0;       /* highest index that has been computed */

/* enlarge the tables so that they can index up to max_idx (inclusive) */
static void ensure_capacity(idx_t max_idx)
{
    /* also need space for indices up to 5*max_idx because of the a(a(n)) rule   */
    size_t needed = (size_t)(5 * max_idx + 10);   /* small safety margin */
    if (cap >= needed) return;

    /* reallocate the arrays */
    seq_t *a_new   = (seq_t *)realloc(tab_a,   needed * sizeof(seq_t));
    bool   *def_new = (bool  *)realloc(tab_def, needed * sizeof(bool));
    bool   *req_new = (bool  *)realloc(tab_req, needed * sizeof(bool));
    seq_t *val_new  = (seq_t *)realloc(tab_val,  needed * sizeof(seq_t));

    /* abort on allocation failure */
    if (!a_new || !def_new || !req_new || !val_new) abort();

    /* initialise the new cells */
    for (size_t i = cap; i < needed; ++i) {
        def_new[i] = false;
        req_new[i] = false;
    }

    tab_a   = a_new;
    tab_def = def_new;
    tab_req = req_new;
    tab_val = val_new;
    cap     = needed;
}

/* initialise the tables for the very first call to a() */
static void initialise(void)
{
    if (tab_a) return;               /* already initialized */

    ensure_capacity(0);              /* allocate the smallest table */
    tab_a[0]   = 0;                   /* a(0) = 0 */
    tab_def[0] = true;
    /* a(0) does not impose a further requirement because a(a(0)) = a(0) = 0 = 5*0 */
    computed = 0;
}

/* the sequence function */
seq_t a(idx_t n)
{
    initialise();

    /* ensure we can store indices up to 5*n, because of a(a(n)) */
    ensure_capacity(n);              /* keep current tables readable up to n       */
    ensure_capacity(5 * n);          /* also keep indices up to 5*n accessible      */

    /* compute all values from computed+1 up to n */
    if ((size_t)n > computed) {
        for (size_t i = computed + 1; i <= (size_t)n; ++i) {
            seq_t val_i;

            /* if a(i) was forced by a previous constraint */
            if (tab_req[i]) {
                val_i = tab_val[i];
            } else {
                /* start from one above the previous value */
                seq_t cand = tab_a[i - 1] + 1;
                if (cand <= (seq_t)i) cand = (seq_t)i + 1;   /* ensure we don't reuse n itself */

                seq_t required = (seq_t)5 * (seq_t)i;      /* desired value of a(a(i))  */
                while (true) {
                    /* candidate larger than required would never work */
                    if (cand > required) abort();

                    size_t idx_cand = (size_t)cand;

                    /* if a(cand) is already defined, it has to equal required   */
                    if (tab_def[idx_cand] && tab_a[idx_cand] != required) {
                        ++cand; continue;
                    }
                    /* if a(cand) has a pending requirement, it must match */
                    if (tab_req[idx_cand] && tab_val[idx_cand] != required) {
                        ++cand; continue;
                    }
                    break;   /* candidate is acceptable */
                }
                val_i = cand;
            }

            tab_a[i]   = val_i;
            tab_def[i] = true;

            /* enforce the relation a(a(i)) = 5*i for the index val_i          */
            seq_t req_val = (seq_t)5 * (seq_t)i;
            size_t idx_req = (size_t)val_i;

            if (tab_def[idx_req]) {               /* already known value */
                if (tab_a[idx_req] != req_val) abort();
            } else {                              /* impose requirement */
                if (tab_req[idx_req]) {            /* conflicting requirement? */
                    if (tab_val[idx_req] != req_val) abort();
                } else {
                    tab_req[idx_req] = true;
                    tab_val[idx_req] = req_val;
                }
            }
        }
        computed = (size_t)n;
    }

    return tab_a[(size_t)n];
}
