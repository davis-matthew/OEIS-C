/*
 * A102039  a(n) = a(n-1) + last digit of a(n-1), starting at 1.
 * https://oeis.org/A102039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = a(n-1) + (a(n-1) mod 10), a(1) = 1
 * Range:     u8 n <= 52, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* array of computed values */
    static idx_t size = 0;         /* number of values in seq (max n computed) */
    static idx_t capacity = 0;     /* allocated capacity */

    /* Helper to grow the array */
    if (n > size) {
        if (capacity == 0) {
            capacity = 8;
            seq = (seq_t *)realloc(NULL, capacity * sizeof(seq_t));
        }
        while (capacity < n) {
            capacity *= 2;
            seq = (seq_t *)realloc(seq, capacity * sizeof(seq_t));
        }

        /* initialize a(1) if this is the first call */
        if (size == 0) {
            seq[0] = (seq_t)1;
            size = 1;
        }

        /* compute missing terms up to n */
        for (idx_t i = size; i < n; ++i) {
            seq_t prev   = seq[i - 1];
            seq_t digit  = prev % (seq_t)10;
            seq[i] = prev + digit;
        }
        size = n;
    }

    return seq[n - 1];
}
