/*
 * A152077  Length of the trajectory of the map x->A003132(x) started at x=n^2
 *          up to the end of its first period.
 * https://oeis.org/A152077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the trajectory of the map digit_square_sum
 *            starting at n² until a value repeats
 * Range:     u8 n <= 15, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* sum of squares of the decimal digits of x */
static seq_t digit_square_sum(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        seq_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety; offset is 1 */

    /* 2048 is far more than enough for any n;
       the orbit quickly falls below 1600, so the set of
       distinct values is tiny. */
    seq_t visited[2048];
    size_t count = 0;

    seq_t value = (seq_t)n * (seq_t)n;   /* start at n² */

    for (;;) {
        /* check if value has already appeared */
        size_t i;
        for (i = 0; i < count; ++i) {
            if (visited[i] == value) {
                return (seq_t)count;
            }
        }
        /* record new value and continue */
        visited[count++] = value;
        value = digit_square_sum(value);
    }
}
