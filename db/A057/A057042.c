/*
 * A057042  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          the n-th Fibonacci number is in antidiagonal a(n).
 * https://oeis.org/A057042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((sqrt(8*F_n + 1) - 1)/2), where F_n is the n-th
 *            Fibonacci number. This gives the index k of the triangular
 *            interval containing F_n
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Compute the n-th term of sequence A057042. */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1; the function is never called with n == 0. */
    seq_t f_prev = 0;          /* F_0 */
    seq_t f_curr = 1;          /* F_1   */
    idx_t count = 0;
    seq_t a_value = 0;

    while (count < n) {
        /* k = ceil((sqrt(8*F + 1) - 1)/2) */
        seq_t X = f_curr * (seq_t)8 + (seq_t)1;      /* 8*F + 1   */
        seq_t s = floor_sqrt(X);                     /* floor(sqrt(X)) */

        seq_t k;
        if (s * s == X) {
            /* perfect square: k = (s - 1) / 2 */
            k = (s - (seq_t)1) / (seq_t)2;
        } else {
            /* not perfect square: k = (s + 1) / 2 */
            k = (s + (seq_t)1) / (seq_t)2;
        }

        a_value = k;
        ++count;

        /* advance Fibonacci: F_{m+1} = F_{m-1} + F_m */
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;
    }

    return a_value;
}
