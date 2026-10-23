/*
 * A164020  Denominators of Bernoulli numbers interleaved with even numbers.
 * https://oeis.org/A164020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 179, u64 n <= 539,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A164020: Denominators of Bernoulli numbers interleaved with even numbers.
   a(2*n) = denominator of B_{2n} (A002445),  a(2*n+1) = 2*(n+1).
*/

#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_initialized = false;

/* Helper to ensure the list of primes up to 10001 is available. */
static void ensure_primes(void)
{
    if (!primes_initialized) {
        /* maximum value needed is 2*n + 1 where n <= 5000 => 10001 */
        build_primes_upto(10001, &prime_list, &prime_count);
        primes_initialized = true;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Even indices: denominators of Bernoulli numbers */
    if ((n & 1u) == 0) {          /* n is even */
        idx_t m = n >> 1;         /* n = 2*m */
        if (m == 0)
            return 1;             /* a(0) = 1 */

        idx_t d = 2 * m;          /* d = 2*m = index of Bernoulli number */
        ensure_primes();

        seq_t denom = 1;
        /* For each prime p <= d+1 check if (p-1) divides d */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p > d + 1)           /* no further primes needed */
                break;
            if ((d % (p - 1)) == 0)
                denom *= (seq_t)p;
        }
        return denom;
    }
    /* Odd indices: a(2*n+1) = 2*(n+1) -> simplifies to n+1 for odd n */
    return (seq_t)(n + 1);
}
