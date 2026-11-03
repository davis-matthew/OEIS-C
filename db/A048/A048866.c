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
 * Validated: standard, pass, 92 terms through n = 917 (2026-11-03)
 * Helpers:   gcd, is_prime_u128, is_prime_u64, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities from the build */
#include <stddef.h>
#include <limits.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/totient.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

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
