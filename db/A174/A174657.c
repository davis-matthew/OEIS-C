/*
 * A174657  Balanced ternary numbers with more negative trits than positive
 *          trits.
 * https://oeis.org/A174657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 9873, u32 n <= 9873, u64 n <= 9873,
 *            u128 n <= 9873, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced ternary numbers with more negative trits than positive trits
   (OEIS A174657).  a(n) is the nth positive integer whose balanced
   ternary representation has a negative sum of digits.
   The implementation scans the natural numbers, tests each value
   for a negative digit sum, and returns the nth one. */

#include <stddef.h>

/* Return the (signed) sum of the balanced ternary digits of x. */
static int balanced_sum(seq_t x)
{
    int sum = 0;
    while (x != (seq_t)0)
    {
        unsigned r = (unsigned)(x % (seq_t)3);
        x /= (seq_t)3;

        if (r == 0)
            /* nothing */;
        else if (r == 1)
            sum += 1;
        else /* r == 2 -> -1 with a carry */
        {
            sum -= 1;
            x += (seq_t)1;
        }
    }
    return sum;
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;
    idx_t count = 0;

    while (1)
    {
        if (balanced_sum(val) < 0)
        {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
