/*
 * A184958  Number of nonincreasing even cycles in all permutations of
 *          {1,2,...,n}. A cycle (b(1), b(2), ...) is said to be increasing
 *          if, when written with its smallest element in the first position,
 *          it satisfies b(1)<b(2)<b(3)<... . A cycle is said to be even if it
 *          has an even number of entries.
 * https://oeis.org/A184958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A184958:
 * a(0) = 0 , a(1) = 0
 * a(2k)   = (2k)*(2k-1)*a(2k-2) + (2k-1)! - 1
 * a(2k+1) = (2k+1)*a(2k)          (i.e. a(n) = n * a(n-1) for odd n) */
#include <stddef.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;                  /* a(0) and a(1) are 0 */
    seq_t fact = 1;                        /* 0! */
    seq_t a_prevprev = 0;                  /* a(0) */
    seq_t a_prev = 0;                      /* a(1) */
    seq_t a_current = 0;

    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;                  /* i! */

        if (i & 1) {                       /* odd i : a(i) = i * a(i-1) */
            a_current = (seq_t)i * a_prev;
        } else {                           /* even i : a(i) = i*(i-1)*a(i-2) + (i-1)! - 1 */
            seq_t term = fact / (seq_t)i;  /* (i-1)! */
            a_current = (seq_t)i * (seq_t)(i-1) * a_prevprev + term - 1;
        }

        a_prevprev = a_prev;               /* shift for next iteration */
        a_prev = a_current;
    }

    return a_prev;                         /* a(n) */
}
