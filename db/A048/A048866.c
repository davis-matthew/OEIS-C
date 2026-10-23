/*
 * A048866  Difference between number of nonprimes and primes in reduced
 *          residue system of n.
 * https://oeis.org/A048866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = φ(n) - 2 * (number of primes in the reduced residue
 *            system of n)
 * Range:     i8 n <= 221, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   gcd, isPrimeGeneric, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities from the build */
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/phi.h"          /* seq_t calc_phi(seq_t m); */
#include "../utils/isPrimeGeneric.h" /* bool isPrimeGeneric(seq_t n); */

/* Compute a(n) for the OEIS sequence A048866 */
seq_t a(idx_t n)
{
    /* a(1) = 1 by definition; the general formula also gives 1 */
    if (n == (idx_t)1) return (seq_t)1;

    /* Count how many residues 1 <= r < n are coprime to n */
    idx_t coprime_cnt = 0;
    /* Count how many of those residues are prime */
    seq_t prime_cnt = 0;

    const seq_t sn = (seq_t)n;               /* convert once for speed */
    for (idx_t r = 1; r < n; ++r) {
        if (gcd((seq_t)r, sn) == (seq_t)1) { /* r is coprime to n */
            ++coprime_cnt;
            if (isPrimeGeneric((seq_t)r))
                ++prime_cnt;
        }
    }

    /* φ(n) equals coprime_cnt, but we can also compute it via calc_phi */
    /* seq_t phi_n = calc_phi(sn); */
    /* Using the counted value avoids an extra function call */
    const seq_t phi_n = (seq_t)coprime_cnt;

    /* a(n) = φ(n) - 2 * prime_cnt */
    return phi_n - (seq_t)(2 * prime_cnt);
}
