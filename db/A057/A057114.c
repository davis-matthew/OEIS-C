/*
 * A057114  Permutation of N induced by the order-preserving permutation of
 *          the rational numbers (x -> x+1); positions in Stern-Brocot tree.
 * https://oeis.org/A057114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of ( fraction at position n ) + 1 in the full
 *            (extended) Stern–Brocot tree
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   ilog2_idx_to_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ilog2_idx_to_i32.h"

/* Compute the numerator and denominator of the fraction that
   occupies index n in the extended Stern–Brocot tree.
   Results are returned in *num and *den (signed long long).   */
static void fraction_at_index(idx_t n, long long *num, long long *den)
{
    long long left_num  = -1;  /* -1/0  (representing -∞)     */
    long long left_den  =  0;
    long long right_num =  1;  /*  1/0  (representing +∞)     */
    long long right_den =  0;
    long long cur_num   =  0;  /* 0/1  (root of the tree) */
    long long cur_den   =  1;

    if (n > 1) {
        int msb = ilog2_idx_to_i32(n);
        idx_t mask = ((idx_t)1 << (msb - 1));

        while (mask) {
            if ((n & mask) == 0) {                 /* go left  */
                long long new_num = left_num + cur_num;
                long long new_den = left_den + cur_den;
                /* right neighbor becomes the old node            */
                right_num = cur_num;
                right_den = cur_den;
                cur_num   = new_num;
                cur_den   = new_den;
            } else {                                 /* go right */
                long long new_num = cur_num + right_num;
                long long new_den = cur_den + right_den;
                /* left neighbor becomes the old node             */
                left_num = cur_num;
                left_den = cur_den;
                cur_num   = new_num;
                cur_den   = new_den;
            }
            mask >>= 1;
        }
    }
    *num = cur_num;
    *den = cur_den;
}

/* Find the index of a fraction (num/den) in the full Stern–Brocot tree.
   The fraction must be reduced and have positive denominator.          */
static idx_t index_of_fraction(long long num, long long den)
{
    idx_t idx = 1;
    long long left_num  = -1;  long long left_den  = 0;
    long long right_num =  1;  long long right_den = 0;
    long long cur_num   =  0;  long long cur_den   = 1;

    if (num == cur_num && den == cur_den)
        return idx;

    while (1) {
        /* compare target (num/den) with current node (cur_num/cur_den) */
        __int128 left_cmp  = (__int128)num   * cur_den;
        __int128 right_cmp = (__int128)cur_num * den;

        if (left_cmp > right_cmp) {            /* target > cur => go right */
            left_num  = cur_num;
            left_den  = cur_den;
            long long new_num = cur_num + right_num;
            long long new_den = cur_den + right_den;
            cur_num = new_num;
            cur_den = new_den;
            idx = (idx << 1) | 1;
        } else {                               /* target < cur => go left */
            right_num = cur_num;
            right_den = cur_den;
            long long new_num = left_num + cur_num;
            long long new_den = left_den + cur_den;
            cur_num = new_num;
            cur_den = new_den;
            idx <<= 1;
        }
        if (num == cur_num && den == cur_den)
            return idx;
    }
}

/* Main function: a(idx_t n) as required by the problem statement.   */
seq_t a(idx_t n)
{
    long long num, den;
    fraction_at_index(n, &num, &den);

    /* fraction + 1 */
    long long target_num = num + den;  /* numerator after adding 1 */
    long long target_den = den;        /* denominator unchanged */

    idx_t idx = index_of_fraction(target_num, target_den);
    return (seq_t)idx;
}
