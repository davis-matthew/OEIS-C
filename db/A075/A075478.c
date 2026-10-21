/*
 * A075478  Number of iteration that first becomes smaller than the initial
 *          value if Collatz-function (A006370) is iterated, starting with
 *          numbers of form 64n+27. Corresponds to selection of every 16th
 *          term from A074474.
 * https://oeis.org/A075478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + number of Collatz iterations (using the standard
 *            Collatz function f(x) = 3x+1 for odd x, f(x) = x/2 for even x)
 *            required to reach a value smaller than the starting value
 *            64*n+27 (n≥0). The sequence is defined only for n≥0
 * Range:     u8 none, u16 n <= 27, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t start = (seq_t)64 * (seq_t)n + (seq_t)27;   /* starting value 64n+27 */
    seq_t cur    = start;                           /* current value in the trajectory */
    idx_t  steps = 0;                                /* number of f-iterations performed */

    /* iterate until the first term is strictly smaller than the start */
    while (cur >= start)
    {
        if (cur & 1)           /* odd: f(x) = 3*x + 1 */
        {
            /* use unsigned __int128 for the temporary product to avoid overflow */
            unsigned __int128 tmp = (unsigned __int128)cur * 3 + 1;
            cur = (seq_t)tmp;
        }
        else                   /* even: f(x) = x / 2 */
        {
            cur >>= 1;
        }
        ++steps;
    }

    /* the required output counts the starting term as the first
       term of the sequence, hence the final +1. */
    return (seq_t)steps + 1;
}
