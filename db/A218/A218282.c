/*
 * A218282  Hilltop maps: number of nX2 binary arrays indicating the locations
 *          of corresponding elements not exceeded by any horizontal or
 *          vertical neighbor in a random 0..4 nX2 array.
 * https://oeis.org/A218282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)+3*a(n-2)+...+3*a(n-7)+2*a(n-8)+2*a(n-9)
 *            -a(n-10)-a(n-11)-...-a(n-17) Initial values: a(1)=3, a(2)=15,
 *            a(3)=63, a(4)=255, a(5)=1019, a(6)=4073, a(7)=16289, a(8)=65153,
 *            a(9)=260609, a(10)=1042417, a(11)=4169595, a(12)=16678079,
 *            a(13)=66711107, a(14)=266839591, a(15)=1067339019, a(16...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_values[17] = {
    (seq_t)3ULL,
    (seq_t)15ULL,
    (seq_t)63ULL,
    (seq_t)255ULL,
    (seq_t)1019ULL,
    (seq_t)4073ULL,
    (seq_t)16289ULL,
    (seq_t)65153ULL,
    (seq_t)260609ULL,
    (seq_t)1042417ULL,
    (seq_t)4169595ULL,
    (seq_t)16678079ULL,
    (seq_t)66711107ULL,
    (seq_t)266839591ULL,
    (seq_t)1067339019ULL,
    (seq_t)4269278705ULL,
    (seq_t)17076805345ULL
};

seq_t a(idx_t n) {
    if (n <= 17) {
        return base_values[n - 1];
    }

    /* Allocate array to hold all computed terms up to n */
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!seq) {
        /* In real code you might handle the allocation error.
           For simplicity we return 0. */
        return (seq_t)0;
    }

    /* Initialize base values */
    for (idx_t i = 1; i <= 17; ++i) {
        seq[i] = base_values[i - 1];
    }

    /* Compute terms from 18 up to n */
    for (idx_t i = 18; i <= n; ++i) {
        /* calculate a(i) via the recurrence */
        seq_t acc = (seq_t)0;

        /* terms with coefficient +3 for a(i-1)..a(i-7) */
        for (idx_t k = 1; k <= 7; ++k) {
            acc += (seq_t)3 * seq[i - k];
        }
        /* terms with coefficient +2 for a(i-8), a(i-9) */
        for (idx_t k = 8; k <= 9; ++k) {
            acc += (seq_t)2 * seq[i - k];
        }
        /* terms with coefficient -1 for a(i-10)..a(i-17) */
        for (idx_t k = 10; k <= 17; ++k) {
            acc -= seq[i - k];
        }

        seq[i] = acc;
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
