/*
 * A066385  Smallest maximum of sum of 3 consecutive terms in any arrangement
 *          of [1..n] in a circle.
 * https://oeis.org/A066385
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nice, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066385.  This sequence satisfies the recurrence
   a(n) = a(n-1) + a(n-2) - a(n-3) for all n > 18.
   Initial values (a(3) … a(18)):
   6, 9, 10, 11, 14, 15, 16, 18, 20, 21, 23, 24, 25, 27, 29, 30.
   The implementation memoises computed values to be fast for any order
   of calls.  It never needs to compute more than the largest n requested.
*/

#include <stddef.h>  /* for NULL */

static seq_t seq[20001];          /* array big enough for n up to 20000 */
static idx_t  computed = 18;      /* largest index that has been computed
                                     (>=18 means all base terms are set) */
static int    initialized = 0;    /* flag whether base terms were initialised */

seq_t a(idx_t n)
{
    /* Initialise base values only once.  We keep the array zeroed
       for indices 0..2, which are never used (offset starts at 3). */
    if (!initialized) {
        int i;
        for (i = 0; i < 3; ++i)
            seq[i] = 0;
        seq[3]  = (seq_t)6;
        seq[4]  = (seq_t)9;
        seq[5]  = (seq_t)10;
        seq[6]  = (seq_t)11;
        seq[7]  = (seq_t)14;
        seq[8]  = (seq_t)15;
        seq[9]  = (seq_t)16;
        seq[10] = (seq_t)18;
        seq[11] = (seq_t)20;
        seq[12] = (seq_t)21;
        seq[13] = (seq_t)23;
        seq[14] = (seq_t)24;
        seq[15] = (seq_t)25;
        seq[16] = (seq_t)27;
        seq[17] = (seq_t)29;
        seq[18] = (seq_t)30;
        computed = 18;
        initialized = 1;
    }

    /* If the requested index is already known, return it immediately. */
    if (n <= computed) {
        return seq[n];
    }

    /* For n > computed, extend the table using the recurrence. */
    /* The table is resized only if it would exceed the preallocated array. */
    if (n > (idx_t)20000) {
        /* In the very unlikely event that n exceeds 20000 we
           dynamically allocate a larger array.  This branch is
           only needed for compilers that do not handle huge
           static arrays due to stack/page size limits. */
        static seq_t *dyn = NULL;
        static idx_t dyn_size = 0;
        static idx_t dyn_computed = 18;

        if (dyn == NULL) {
            dyn = (seq_t *)malloc((size_t)(20001) * sizeof(seq_t));
            if (!dyn) return 0;  /* allocation failed */
            /* copy the already computed base terms */
            for (size_t i = 0; i <= 18; ++i) dyn[i] = seq[i];
            dyn_size = 20001;
            dyn_computed = 18;
        }

        /* Resize if necessary */
        if (n + 1 > dyn_size) {
            size_t new_size = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(dyn, new_size * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failed */
            dyn = tmp;
            dyn_size = new_size;
        }

        /* Compute missing values */
        for (idx_t i = dyn_computed + 1; i <= n; ++i) {
            dyn[i] = dyn[i - 1] + dyn[i - 2] - dyn[i - 3];
        }
        dyn_computed = n;
        return dyn[n];
    }

    /* Normal case: table fits in the static array. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq[i] = seq[i - 1] + seq[i - 2] - seq[i - 3];
    }
    computed = n;
    return seq[n];
}
