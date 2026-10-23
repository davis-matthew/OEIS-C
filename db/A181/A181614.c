/*
 * A181614  a(n) = number of steps needed to reach a Harshad number when the
 *          map k->A062028(l) is iterated starting at n, or -1 if a Harshad
 *          number is never reached.
 * https://oeis.org/A181614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  a(n) = number of steps needed to reach a Harshad number under the
 *  Harshad transformation f(x) = x + digit_sum(x).  If a Harshad number
 *  is reached immediately the value is 0.
 *  A Harshad number is one divisible by the sum of its decimal digits.
 */
#include <stdbool.h>

static seq_t digit_sum(seq_t x)
{
    seq_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

static bool is_harshad(seq_t x)
{
    seq_t s = digit_sum(x);
    return s != 0 && (x % s) == 0;
}

seq_t a(idx_t n)
{
    /* Convert index to the calculation type */
    seq_t cur = n;
    seq_t steps = 0;

    /* If the starting number is already Harshad, zero steps are needed. */
    while (!is_harshad(cur)) {
        /* Apply the Harshad transformation */
        cur += digit_sum(cur);
        ++steps;
    }

    return steps;
}
