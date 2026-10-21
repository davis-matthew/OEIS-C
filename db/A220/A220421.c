/*
 * A220421  Number of halving and tripling steps to reach the largest value in
 *          the Collatz (3x+1) trajectory of n.
 * https://oeis.org/A220421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Collatz (3x+1) steps required to reach the
 *            first occurrence of the maximum value in the trajectory starting
 *            from n
 * Range:     u8 n <= 26, u16 n <= 1790, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for integer types if needed */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, return 0 to be safe */

    seq_t cur   = (seq_t)n;   /* current value in the Collatz sequence */
    seq_t max   = cur;        /* maximum value seen so far */
    seq_t steps = 0;          /* number of steps taken so far */
    seq_t result = 0;         /* steps to reach the maximum */

    while (cur != 1)
    {
        /* Apply Collatz rule: odd -> 3*cur+1, even -> cur/2 */
        if (cur & (seq_t)1)          /* test oddness */
            cur = 3 * cur + 1;
        else
            cur = cur / 2;

        steps++;                      /* one step just taken */

        /* If a new maximum is found, record the number of steps to reach it */
        if (cur > max)
        {
            max   = cur;
            result = steps;
        }
    }

    return result;
}
