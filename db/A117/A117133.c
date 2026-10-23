/*
 * A117133  Dimensions of the traditional Cartan exceptional group sequence
 *          A1, G2, F4, E6, E7, E8.
 * https://oeis.org/A117133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, uned
 * Formula:   a(n) = 2 * sum_of_exponents(n) + rank(n) where exponents(n) and
 *            rank(n) are the exponents and rank of the n-th traditional
 *            Cartan exceptional Lie algebra A1, G2, F4, E6, E7, E8 for n =
 *            1..6
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Exponent lists for each exceptional group */
    static const int exps_A1[] = { 1 };
    static const int exps_G2[] = { 1, 5 };
    static const int exps_F4[] = { 1, 5, 7, 11 };
    static const int exps_E6[] = { 1, 4, 5, 7, 8, 11 };
    static const int exps_E7[] = { 1, 5, 7, 9, 11, 13, 17 };
    static const int exps_E8[] = { 1, 7, 11, 13, 17, 19, 23, 29 };

    /* Structures describing each group */
    typedef struct {
        int rank;
        int count;
        const int *exps;
    } group_t;

    static const group_t groups[6] = {
        { 1,  1, exps_A1  },
        { 2,  2, exps_G2  },
        { 4,  4, exps_F4  },
        { 6,  6, exps_E6  },
        { 7,  7, exps_E7  },
        { 8,  8, exps_E8  }
    };

    if (n < 1 || n > 6u) {
        return (seq_t)0;   /* undefined beyond the exceptional list */
    }

    const group_t *g = &groups[n - 1u];
    seq_t sum = 0;
    for (size_t i = 0; i < (size_t)g->count; ++i) {
        sum += (seq_t)g->exps[i];
    }
    return sum * 2 + (seq_t)g->rank;
}
