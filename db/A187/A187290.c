/*
 * A187290  Number of 5-step one or two space at a time rook's tours on an n X
 *          n board summed over all starting positions.
 * https://oeis.org/A187290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=0, a(2)=0, a(3)=720, a(4)=4576, a(5)=13640, a(6)=28224, a(7)=48232,
   a(8)=73408, a(9)=103692, a(10)=139056
   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3)   for n>10
   Empirical (valid for n>7): a(n) = 2540*n^2 - 12896*n + 14016
*/
#include <stddef.h>

static const seq_t init_arr[11] = {
    0,          /* dummy for index 0 */
    0,          /* 1 */
    0,          /* 2 */
    720,        /* 3 */
    4576,       /* 4 */
    13640,      /* 5 */
    28224,      /* 6 */
    48232,      /* 7 */
    73408,      /* 8 */
    103692,     /* 9 */
    139056      /*10 */
};

seq_t a(idx_t n)
{
    if (n <= 10) {
        return init_arr[(size_t)n];
    }

    /* compute terms from 11 up to n using the recurrence */
    seq_t a_k_3 = init_arr[8];   /* a(k-3), starting with k=11 so a(8) */
    seq_t a_k_2 = init_arr[9];   /* a(k-2) */
    seq_t a_k_1 = init_arr[10];  /* a(k-1) */
    seq_t a_k;                   /* current term */

    for (idx_t k = 11; k <= n; ++k) {
        a_k = 3 * a_k_1 - 3 * a_k_2 + a_k_3;
        a_k_3 = a_k_2;
        a_k_2 = a_k_1;
        a_k_1 = a_k;
    }
    return a_k;
}
