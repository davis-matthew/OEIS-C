/*
 * A162897  a(1)=a(2)=2. a(n) = the smallest integer >= 2 such that
 *          a(n-1)^a(n) > a(n-2)^a(n-1).
 * https://oeis.org/A162897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162897
   a(1)=2, a(2)=2, a(3)=3, a(4)=2, a(5)=4, a(6)=3.
   For n >= 7, a(n) = ceil(n/2), i.e. (n+1)/2 using integer division. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms explicitly */
    if (n <= 6) {
        static const seq_t first[7] = {0, 2, 2, 3, 2, 4, 3};
        return first[n];
    }
    /* For n >= 7, a(n) = ceil(n / 2) */
    return ((seq_t)(n + 1)) / 2;
}
