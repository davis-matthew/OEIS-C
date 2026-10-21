/*
 * A227367  a(0)=1, a(n+1) = a(0) + a(1)*a(2) + a(3)*a(4)*a(5) +
 *          a(6)*a(7)*a(8)*a(9) + ... + ...*a(n).
 * https://oeis.org/A227367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1
 *            a(n+1)=a(0)+a(1)*a(2)+a(3)*a(4)*a(5)+a(6)*a(7)*a(8)*a(9)+…+product
 *            of a(i) where the products are taken over groups of size
 *            1,2,3,4,… respectively
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    /* cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;          /* next index to compute */

    /* state of the current group being filled */
    static seq_t sumCompleted = 0;      /* sum of products of fully completed groups */
    static seq_t current_prod = 1;      /* product of the current (possibly incomplete) group */
    static idx_t group_size   = 1;      /* size of the current group */
    static idx_t group_rem    = 1;      /* elements still to add to finish the group */
    static bool  group_has    = false;  /* whether the current group has started */

    /* ---------- initialization on first call ---------- */
    if (cache == NULL) {
        size_t init_cap = (size_t)(n + 1);
        if (init_cap < 1) init_cap = 1;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */

        cache[0] = (seq_t)1;                /* a(0) = 1 */

        /* initialise the state as if a(0) had just been processed */
        sumCompleted   = (seq_t)1;          /* group 0 finished */
        current_prod   = (seq_t)1;          /* product of next group starts fresh */
        group_has      = false;             /* no elements of the next group yet */
        group_size     = (idx_t)2;          /* next group will contain 2 elements */
        group_rem      = (idx_t)2;          /* two more elements needed to finish it */
        computed       = (idx_t)1;          /* next term to compute is a(1) */
        cap = init_cap;
    }

    /* ---------- ensure cache large enough ---------- */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < cap * 2) new_cap = cap * 2;   /* exponential growth */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* ---------- compute missing values up to n ---------- */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t val = sumCompleted;
        if (group_has) val += current_prod;   /* add product of the current group so far */
        cache[i] = val;

        /* update group state to include this new value */
        current_prod *= val;
        group_has = true;
        if (group_rem > 0) group_rem--;
        if (group_rem == 0) {                  /* group finished */
            sumCompleted += current_prod;
            current_prod = (seq_t)1;
            group_has   = false;
            group_size++;
            group_rem = group_size;
        }
    }
    computed = n + 1;

    return cache[n];
}
