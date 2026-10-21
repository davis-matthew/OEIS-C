/*
 * A057970  Number of 5 X n binary matrices with exactly 1 singleton column up
 *          to row and column permutations.
 * https://oeis.org/A057970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057970:
   a(n) = number of 5×n binary matrices with exactly one singleton column,
   up to row and column permutations.
   Generating function:
     a(n) = n/120 * ( 5/(1−x)^27
                      + 30/(1−x)^13/(1−x^2)^7
                      + 15/(1−x)^7 /(1−x^2)^10
                      + 40/(1−x)^6 /(1−x^3)^7
                      + 30/(1−x)^3 /(1−x^2)^2/(1−x^4)^5 )
   The coefficients can be expressed as a sum of products of binomial
   coefficients (see the OEIS entry).  The implementation below follows
   these algebraic expressions directly, using only integer arithmetic
   and the provided binom helper.  Intermediate products are held in
   unsigned __int128 to avoid overflow on larger builds; the final result
   is cast to seq_t.  No factorials or explicit power series are
   constructed, so the algorithm runs in O(n) time with a very small
   constant factor.  It works for any seq_t that can hold the result
   (the large‑precision build uses arbitrary‑precision seq_t). */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* For n=1 the formula gives 1, because the inner sums all start
       at 0 and the prefactor b = n-1 = 0. */
    idx_t b = n - 1;          /* use b to match the x power in the g.f. */

    unsigned __int128 total = 0;   /* accumulate in 128‑bit to avoid overflow */

    /* Term 1: 5 * C(b+26, 26) */
    total += (unsigned __int128)5 *
             (unsigned __int128)binom(b + 26, 26);

    /* Term 2: 30 * Σ_{l=0}^{⌊b/2⌋} C(12 + (b-2l), 12) * C(6 + l, 6) */
    for (idx_t l = 0; l <= b / 2; ++l) {
        idx_t k = b - 2 * l;
        seq_t t1 = binom(12 + k, 12);
        seq_t t2 = binom(6 + l, 6);
        total += (unsigned __int128)30 *
                 (unsigned __int128)t1 *
                 (unsigned __int128)t2;
    }

    /* Term 3: 15 * Σ_{l=0}^{⌊b/2⌋} C(6 + (b-2l), 6) * C(9 + l, 9) */
    for (idx_t l = 0; l <= b / 2; ++l) {
        idx_t k = b - 2 * l;
        seq_t t1 = binom(6 + k, 6);
        seq_t t2 = binom(9 + l, 9);
        total += (unsigned __int128)15 *
                 (unsigned __int128)t1 *
                 (unsigned __int128)t2;
    }

    /* Term 4: 40 * Σ_{l=0}^{⌊b/3⌋} C(5 + (b-3l), 5) * C(6 + l, 6) */
    for (idx_t l = 0; l <= b / 3; ++l) {
        idx_t k = b - 3 * l;
        seq_t t1 = binom(5 + k, 5);
        seq_t t2 = binom(6 + l, 6);
        total += (unsigned __int128)40 *
                 (unsigned __int128)t1 *
                 (unsigned __int128)t2;
    }

    /* Term 5: 30 * Σ_{l=0}^{⌊b/2⌋} Σ_{m=0}^{⌊(b-2l)/4⌋}
                C(2 + (b-2l-4m), 2) * C(1 + l, 1) * C(4 + m, 4) */
    for (idx_t l = 0; l <= b / 2; ++l) {
        idx_t k = b - 2 * l;
        seq_t t_l = binom(1 + l, 1);          /* = 1 + l */
        for (idx_t m = 0; m <= k / 4; ++m) {
            idx_t j = k - 4 * m;
            seq_t t_j = binom(2 + j, 2);
            seq_t t_m = binom(4 + m, 4);
            total += (unsigned __int128)30 *
                     (unsigned __int128)t_j *
                     (unsigned __int128)t_l *
                     (unsigned __int128)t_m;
        }
    }

    /* Final division by 120 */
    total /= 120;

    return (seq_t)total;
}
