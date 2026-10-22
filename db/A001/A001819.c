/*
 * A001819  Central factorial numbers: second right-hand column of triangle
 *          A008955.
 * https://oeis.org/A001819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1)*n^2 + ((n-1)!)^2, a(0) = 0 (i.e. a(n) = a(n-1)*n^2
 *            + A001044(n-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* State: A = a(k), F = (k!)^2 for k = last */
    static idx_t last = 0;
    static seq_t A = 0;
    static seq_t F = 1;

    if (n < last) {
        last = 0;
        A = 0;
        F = 1;
    }
    while (last < n) {
        idx_t i = last + 1;
        seq_t sq = (seq_t)i * (seq_t)i;
        A = A * sq + F;
        F = F * sq;
        last = i;
    }
    return A;
}
