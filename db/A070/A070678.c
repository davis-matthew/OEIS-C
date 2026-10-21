/*
 * A070678  Smallest m in range 1..phi(n) such that 7^m == 1 mod n, or 0 if no
 *          such number exists.
 * https://oeis.org/A070678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest m ∈ [1..φ(n)] such that 7^m ≡ 1 (mod n), or 0 if no such m exists. */

/* This routine computes the multiplicative order of 7 modulo n
   by brute‑force testing of increasing exponents.
   It uses the provided helper functions:
     - gcd(seq_t a, seq_t b)
     - calc_phi(seq_t m)
     - mod_pow(seq_t base, uint64_t exp, seq_t mod)
*/

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/calc_phi.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* n is at least 1.  For safety we handle n=0. */
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;

    /* If 7 is not coprime to n, there is no solution. */
    seq_t g = gcd((seq_t)7, sn);
    if (g != (seq_t)1)
        return 0;

    /* φ(n) is the upper bound for the order. */
    seq_t phi_seq = calc_phi(sn);
    uint64_t phi = (uint64_t)phi_seq;

    /* Search for the smallest exponent m with 7^m ≡ 1 (mod n). */
    for (uint64_t m = 1; m <= phi; ++m) {
        if (mod_pow((seq_t)7, m, sn) == (seq_t)1)
            return (seq_t)m;
    }

    /* No exponent found – should not happen for coprime n
       but return 0 to satisfy the definition. */
    return 0;
}
