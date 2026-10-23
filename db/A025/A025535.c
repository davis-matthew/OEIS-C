/*
 * A025535  a(n) = (1/C(2n,0) - 1/C(2n,1) + ... + d/C(2n,2n))*L, where d =
 *          (-1)^2n, L = LCM{C(2n,0), C(2n,1),..., C(2n,2n)}.
 * https://oeis.org/A025535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = LCM{n+1, n+2, ..., 2*n+1}/(n+1) (OEIS A025535)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   build_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External utilities from the build environment */
#include "../utils/gcd.h"        /* not used directly, kept for potential use */
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* Persistent list of primes up to the current maximum needed */
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    /* Endpoints of the interval [n+1, 2n+1] */
    idx_t a_start = n + 1;
    idx_t b_end   = 2 * n + 1;

    /* Re‑compute the prime list if we need a larger bound */
    if (b_end > primes_limit) {
        /* free previous allocation if any */
        free(primes);
        build_primes(b_end, &primes, &primes_cnt);
        primes_limit = b_end;
    }

    /* Accumulate the LCM by tracking the maximal exponent of each prime in
       the interval.  No temporary big integers are stored here; multiplication
       is done directly on the result. */
    seq_t lcm = 1;

    for (size_t i = 0; i < primes_cnt; ++i)
    {
        idx_t p = primes[i];
        if (p > b_end) break;          /* safety guard */

        /* Determine the highest power p^e <= b_end */
        idx_t pw = p;   /* current power */
        int    e  = 1;   /* current exponent */

        while (pw <= b_end / p) {
            pw *= p;
            ++e;
        }

        /* Adjust exponent if there is no multiple of p^e within [a_start, b_end] */
        while (e > 0 && ( (a_start + pw - 1) / pw ) > (b_end / pw) ) {
            --e;
            pw /= p;
        }

        if (e > 0) {
            /* Multiply lcm by p^e.  As e is small (at most 14 for our limits)
               a simple loop suffices. */
            seq_t pe = 1;
            for (int j = 0; j < e; ++j)
                pe = pe * (seq_t)p;
            lcm = lcm * pe;
        }
    }

    /* Final division by (n+1) gives the required term. */
    return lcm / (seq_t)a_start;
}
