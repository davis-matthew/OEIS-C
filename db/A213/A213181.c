/*
 * A213181  Number of chains of even numbers of length 2 or more in the
 *          Collatz (3x+1) trajectory of n.
 * https://oeis.org/A213181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of chains of consecutive even integers of length ≥
 *            2 in the Collatz (3x+1) trajectory of n
 * Range:     u8 n <= 26, u16 n <= 7790, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)             /* undefined by OEIS, guard against infinite loop */
        return (seq_t)0;

    seq_t x      = (seq_t)n; /* current value in the trajectory      */
    seq_t chains = 0;        /* number of chains found              */
    seq_t run    = 0;        /* length of current run of even numbers*/

    while (1) {
        /* parity check */
        if ((x & (seq_t)1) == 0) {
            /* current value is even, extend the run */
            run++;
        } else {
            /* current value is odd, finish any run */
            if (run >= (seq_t)2)
                chains++;
            run = 0;
        }

        /* reached the stopping value?   */
        if (x == (seq_t)1)
            break;

        /* next Collatz step */
        if ((x & (seq_t)1) == 0)
            x = x / (seq_t)2;
        else
            x = (seq_t)3 * x + (seq_t)1;
    }

    return chains;
}
