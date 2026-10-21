/*
 * A078719  Number of odd terms among n, f(n), f(f(n)), ...., 1 for the
 *          Collatz function (that is, until reaching "1" for the first time),
 *          or -1 if 1 is never reached.
 * https://oeis.org/A078719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of odd terms among n, f(n), f(f(n)), …, 1 where
 *            f(x) is the standard Collatz function: f(x) = x/2 if x is even
 *            f(x) = 3x+1 if x is odd If the chain never reaches 1
 *            (theoretically impossible if the Collatz conjecture is true),
 *            the function returns (seq_t)-1
 * Range:     u8 n <= 26, u16 n <= 4573, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* All valid input indices start at 1. */
    if (n == 0)
        return (seq_t)-1;

    seq_t value = (seq_t)n;   /* Current term of the Collatz sequence.   */
    seq_t odds   = 0;         /* Count of odd terms seen so far.       */
    const idx_t MAX_STEPS = 2000000;  /* Safety limit to avoid infinite loops. */

    /* Count the first term if it is odd. */
    if (value & (seq_t)1)
        odds++;

    idx_t steps = 0;
    while (value != (seq_t)1 && steps < MAX_STEPS) {
        /* Apply the Collatz rule in place. */
        if ((value & (seq_t)1) == 0)          /* even */
            value >>= 1;
        else                                 /* odd  */
            value = 3 * value + 1;

        if (value & (seq_t)1)                /* odd after the step */
            odds++;

        steps++;
    }

    /* If we exited due to hitting 1, return the count; otherwise
       return -1 to indicate the conjecture may be false for this n. */
    return (value == (seq_t)1) ? odds : (seq_t)-1;
}
