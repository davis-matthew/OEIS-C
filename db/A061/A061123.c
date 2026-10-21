/*
 * A061123  Number of degree-n permutations of order exactly 9.
 * https://oeis.org/A061123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_seq_array, binom, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T9[n] - T3[n]
 * where T9[n] and T3[n] count permutations on n symbols with
 * cycle lengths in {1,3,9} and {1,3} respectively.
 * They satisfy the recurrence
 *   T[0] = 1,
 *   T[n] = Σ_{k∈S, k≤n} binom(n-1,k-1) * (k-1)! * T[n-k]
 * with S = {1,3,9} for T9 and S = {1,3} for T3.
 */
#include <stddef.h>
#include "../utils/binom.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* factorials (k-1)! for k = 1,3,9  (indices 0,2,8) */
    static const seq_t fact_small[9] = {
        1ULL,        /* 0!   */
        1ULL,        /* 1!   */
        2ULL,        /* 2!   */
        6ULL,        /* 3!   */
        24ULL,       /* 4!   */
        120ULL,      /* 5!   */
        720ULL,      /* 6!   */
        5040ULL,     /* 7!   */
        40320ULL     /* 8!   */
    };

    /* arrays for the two recurrences */
    seq_t *t9 = alloc_seq_array((size_t)(n + 1));
    seq_t *t3 = alloc_seq_array((size_t)(n + 1));
    t9[0] = 1;
    t3[0] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum9 = 0;
        seq_t sum3 = 0;

        /* k = 1 */
        seq_t term1 = binom(i - 1, 0) * fact_small[0] * t9[i - 1];
        sum9 += term1;
        sum3 += binom(i - 1, 0) * fact_small[0] * t3[i - 1]; /* use t3 here */

        /* k = 3 */
        if (i >= 3) {
            seq_t term3 = binom(i - 1, 2) * fact_small[2] * t9[i - 3];
            sum9 += term3;
            seq_t term3b = binom(i - 1, 2) * fact_small[2] * t3[i - 3];
            sum3 += term3b;
        }

        /* k = 9 */
        if (i >= 9) {
            seq_t term9 = binom(i - 1, 8) * fact_small[8] * t9[i - 9];
            sum9 += term9;
        }

        t9[i] = sum9;
        t3[i] = sum3;
    }

    seq_t result = t9[n] - t3[n];

    free_seq_array(t9);
    free_seq_array(t3);

    return result;
}
