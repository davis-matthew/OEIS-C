/*
 * A060401  a(n) = minimal m such that m>n, n divides m, n-1 divides m-1, n-2
 *          divides m-2 and so on down to 1 divides m-n+1.
 * https://oeis.org/A060401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + lcm(1,2,…,n) (OEIS A060401)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   build_primes_upto, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                  /* for free */
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/pow_int.h"             /* pow_int if needed */

static uint64_t *s_primes = NULL;   /* primes up to the largest n seen */
static size_t   s_pcount = 0;       /* number of primes in the array    */
static idx_t    s_limit  = 0;       /* highest n for which primes were built */

/*--------------------------------------------------------------------
   Returns a(n) = n + lcm(1..n).  The function works for any seq_t
   size (8, 16, 32, 64, 128 bits or arbitrary precision) because it
   only multiplies seq_t values and never exceeds the natural range of
   seq_t for the values that fit into it.  Larger values wrap as
   appropriate for the arithmetic type, which is acceptable for this
   sequence.
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Rebuild the prime table if a larger n is requested */
    if (n > s_limit) {
        if (s_primes != NULL) {
            free(s_primes);
            s_primes = NULL;
        }
        build_primes_upto(n, &s_primes, &s_pcount);
        s_limit = n;
    }

    seq_t lcm = (seq_t)1;               /* will accumulate lcm(1..n) */

    for (size_t i = 0; i < s_pcount; ++i) {
        uint64_t p = s_primes[i];
        if (p > n)                     /* primes beyond n are not needed */
            break;

        /* Find the largest power of p not exceeding n */
        uint64_t power_uint = p;
        while (power_uint <= n / p)    /* avoid overflow on power_uint * p */
            power_uint *= p;

        seq_t power_seq = (seq_t)power_uint;
        lcm *= power_seq;              /* multiply into the lcm accumulator */
    }

    /* a(n) = n + lcm(1..n) */
    return lcm + (seq_t)n;
}
