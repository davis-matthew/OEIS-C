/*
 * A163379  a(n) = phi(phi(tau(n))).
 * https://oeis.org/A163379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(phi(tau(n)))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf, calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   tau(n) = number of divisors of n (A000005)                      */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/calc_phi.h"

/* Compute a(n) = phi(phi(tau(n))) for any n >= 1.
 * The function memoises a smallest‑prime‑factor array so that
 * subsequent calls are fast even when n is not monotonic.    */
seq_t a(idx_t n)
{
    /* static cache for smallest prime factors up to the
       maximum n seen so far                                 */
    static idx_t *spf      = NULL;
    static idx_t spf_limit = 0;

    /* Grow or create the SPF table if necessary            */
    if (n > spf_limit) {
        if (spf != NULL) free(spf);
        build_spf(n, &spf);
        spf_limit = n;
    }

    /* Compute tau(n) = product_{p^e || n} (e+1)             */
    seq_t tau = 1;
    idx_t x   = n;
    while (x > 1) {
        idx_t p   = spf[x];
        int   exp = 0;
        while (x % p == 0) {
            x /= p;
            exp++;
        }
        tau *= (seq_t)(exp + 1);
    }

    /* a(n) = phi( phi( tau(n) ) )                            */
    seq_t t1 = calc_phi(tau);
    seq_t res = calc_phi(t1);
    return res;
}
