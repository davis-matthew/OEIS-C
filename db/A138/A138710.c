/*
 * A138710  n-th run has length n-th positive Fibonacci numbers, with digits 0
 *          and 1 only, starting with 0.
 * https://oeis.org/A138710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A138710: 0–1 runs of Fibonacci lengths, starting with 0
    Each run i has length F(i) (Fib(1)=1, Fib(2)=1, Fib(3)=2, …).
    The first run (i=1) consists of 0s, the second of 1s, the third of 0s, etc.
    Thus the n-th term is 0 if position n lies in an odd‑numbered run,
    otherwise it is 1.
*/

seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but return 0 to keep type safety */
        return (seq_t)0;

    /* Fibonacci initialization: Fib(1)=1, Fib(2)=1 */
    idx_t prev = 0;
    idx_t curr = 1;     /* curr holds Fib(i) */
    idx_t run  = 1;     /* current run index, starts at 1 */
    idx_t remaining = n;

    /* subtract run lengths until the position lies within the current run */
    while (remaining > curr)
    {
        remaining -= curr;
        run++;

        /* compute next Fibonacci number */
        idx_t next = prev + curr;
        prev = curr;
        curr = next;
    }

    /* after loop, position is within run 'run':
       run odd  -> digit 0,
       run even -> digit 1  */
    /* return 0 for odd run (run%2==1), otherwise 1 */
    return (seq_t)(run % 2 == 0);
}
