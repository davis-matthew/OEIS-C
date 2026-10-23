/*
 * A183338  Number of n X 6 binary arrays with each 1 adjacent to exactly one
 *          1 vertically and one 1 horizontally.
 * https://oeis.org/A183338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = a(n-1) + 2*a(n-2) +
 *            9*a(n-3) + 6*a(n-4) - 3*a(n-5) -10*a(n-6) -17*a(n-7) - 7*a(n-8)
 *            +12*a(n-9) +12*a(n-10) - 6*a(n-11) with initial values a(1)=1,
 *            a(2)=9, a(3)=23, a(4)=51, a(5)=175, a(6)=513, a(7)=1397,
 *            a(8)=4133, a(9)=12075, a(10)=34521, a(11)=100047
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    static seq_t *seq = NULL;     /* cached values array */
    static idx_t computed = 0;    /* number of terms stored */

    /* initial 11 terms */
    static const seq_t init[11] = {
        (seq_t)1,
        (seq_t)9,
        (seq_t)23,
        (seq_t)51,
        (seq_t)175,
        (seq_t)513,
        (seq_t)1397,
        (seq_t)4133,
        (seq_t)12075,
        (seq_t)34521,
        (seq_t)100047
    };

    /* first call: allocate space for at least min(11,n) terms */
    if (seq == NULL) {
        computed = (n > 11) ? n : 11;
        seq = (seq_t *)malloc((size_t)computed * sizeof(seq_t));
        if (!seq) return 0; /* allocation failed */
        for (idx_t i = 0; i < 11; ++i)
            seq[i] = init[i];
        /* compute further terms if needed */
        for (idx_t i = 11; i < n; ++i) {
            seq[i] = seq[i-1]
                   + seq[i-2] * (seq_t)2
                   + seq[i-3] * (seq_t)9
                   + seq[i-4] * (seq_t)6
                   + seq[i-9] * (seq_t)12
                   + seq[i-10] * (seq_t)12
                   - seq[i-5] * (seq_t)3
                   - seq[i-6] * (seq_t)10
                   - seq[i-7] * (seq_t)17
                   - seq[i-8] * (seq_t)7
                   - seq[i-11] * (seq_t)6;
        }
        return seq[n-1];
    }

    /* if a larger index is requested, extend the array */
    if (n > computed) {
        idx_t old = computed;
        computed = n;
        seq = (seq_t *)realloc(seq, (size_t)computed * sizeof(seq_t));
        if (!seq) return 0; /* reallocation failed */

        for (idx_t i = old; i < n; ++i) {
            seq[i] = seq[i-1]
                   + seq[i-2] * (seq_t)2
                   + seq[i-3] * (seq_t)9
                   + seq[i-4] * (seq_t)6
                   + seq[i-9] * (seq_t)12
                   + seq[i-10] * (seq_t)12
                   - seq[i-5] * (seq_t)3
                   - seq[i-6] * (seq_t)10
                   - seq[i-7] * (seq_t)17
                   - seq[i-8] * (seq_t)7
                   - seq[i-11] * (seq_t)6;
        }
    }

    return seq[n-1];
}
