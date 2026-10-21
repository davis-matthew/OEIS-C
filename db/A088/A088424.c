/*
 * A088424  Number of primes in arithmetic progression starting with 13 and
 *          with d=2n.
 * https://oeis.org/A088424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes in the arithmetic progression 13 + 2n·k
 *            (k = 0, 1, 2, …) until the first non‑prime term is encountered
 * Range:     u8 n <= 44, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* offset starts at 1 */

    seq_t count = 0;                       /* number of primes found     */
    seq_t d     = (seq_t)2 * (seq_t)n;     /* common difference d = 2n   */
    seq_t term  = 13;                       /* first term of the progression */

    while (isPrimeGeneric(term)) {          /* keep counting primes      */
        ++count;
        term += d;                          /* next term in progression  */
    }
    return count;
}
