/*
 * A064684  Number of primes in orbit of 2n+1 in the 3x+1 problem.
 * https://oeis.org/A064684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes that appear in the Collatz orbit of the
 *            odd start 2*n+1
 * Range:     u8 n <= 1757, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper _is_prime(uint64_t) from the build utilities to test primality. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/_is_prime.h"  /* Ensure the function is declared, if required by the build system */

seq_t a(idx_t n)
{
    /* Count of primes in the orbit */
    seq_t count = 0;
    /* Start value: 2*n+1 (odd number) */
    unsigned __int128 v = (unsigned __int128)(2ULL * (seq_t)n + (seq_t)1);

    while (1) {
        /* Test primality if the current value fits in 64 bits */
        if (v <= UINT64_MAX) {
            if (_is_prime((uint64_t)v))
                count += 1;
        }
        /* Terminate when reaching 1 */
        if (v == 1)
            break;

        /* Collatz step: even -> divide by 2, odd -> 3*n + 1 */
        if ( (v & 1) == 0 )
            v >>= 1;
        else
            v = 3 * v + 1;
    }
    return count;
}
