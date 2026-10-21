/*
 * A076934  Smallest integer of the form n/k!.
 * https://oeis.org/A076934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n / A055881(n)! (largest factorial divisor of n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /** n is given as an index; cast it to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /** current factorial divisor (starting with 1!) */
    seq_t fact = 1;
    /** m is the current factorial index (fact == m!) */
    idx_t m = 1;

    /* Find the largest m such that m! divides N.
       We grow fact step by step, stopping when the next factorial
       would exceed N or fail to divide N. */
    for (;;)
    {
        /* next factorial would be fact * (m+1) == (m+1)! */
        /* If that product would exceed N, stop. */
        if (fact > N / (m + 1))
            break;

        seq_t next = fact * (m + 1);

        if (N % next == 0)
        {   /* still divides; update fact and m */
            fact = next;
            m++;
        }
        else
            break;
    }

    /* Now fact == m! is the largest factorial divisor of N.
       a(n) = N / fact. */
    return N / fact;
}
