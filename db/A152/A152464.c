/*
 * A152464  Number of n-digit bouncy numbers in which every pair of adjacent
 *          digits are distinct.
 * https://oeis.org/A152464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 0 for n <= 2; For n > 2, a(n) = Σ_{i=1..9}
 *            (u(n,i)+d(n,i)) with the recurrence u(1,i) = d(1,i) = 1 for all
 *            i = 0..9 u(n,i) = Σ_{j=i+1..9} d(n-1,j) d(n,i) = Σ_{j=0..i-1}
 *            u(n-1,j) (only the terms with i=1..9 are summed in a(n).)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, required for later utilities */

seq_t a(idx_t n)
{
    if (n < 3) return 0;                /* a(1)=a(2)=0 */

    /* arrays for digits 0..9 */
    seq_t u[10];
    seq_t d[10];

    /* initialise for length 1 */
    for (idx_t i = 0; i <= 9; ++i) {
        u[i] = 1;
        d[i] = 1;
    }

    /* iterate for lengths 2..n */
    for (idx_t len = 2; len <= n; ++len) {
        seq_t next_u[10];
        seq_t next_d[10];

        /* compute next_u[0] = Σ_{j=1..9} d[j] */
        seq_t sum = 0;
        for (idx_t j = 1; j <= 9; ++j) sum += d[j];
        next_u[0] = sum;

        /* next_u[i] for i = 1..9 */
        for (idx_t i = 1; i <= 9; ++i) {
            sum = 0;
            for (idx_t j = i + 1; j <= 9; ++j) sum += d[j];
            next_u[i] = sum;
        }

        /* next_d[i] for i = 0..9  (= Σ_{j=0}^{i-1} u[j]) */
        for (idx_t i = 0; i <= 9; ++i) {
            sum = 0;
            for (idx_t j = 0; j < i; ++j) sum += u[j];
            next_d[i] = sum;
        }

        /* copy back to current arrays */
        for (idx_t i = 0; i <= 9; ++i) {
            u[i] = next_u[i];
            d[i] = next_d[i];
        }
    }

    /* sum over digits 1..9 to get a(n) */
    seq_t total = 0;
    for (idx_t i = 1; i <= 9; ++i) {
        total += u[i];
        total += d[i];
    }
    return total;
}
