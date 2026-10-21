/*
 * A076943  Smallest k > 0 such that n*k + 1 is an n-th power.
 * https://oeis.org/A076943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   mulmod_uint64, pow_seq_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A076943: a(n) = smallest k > 0 such that n*k+1 is an n‑th power.
 *  Computation: for n ≥ 2 find the smallest integer m ≥ 2 with
 *  m^n ≡ 1 (mod n).  Then k = (m^n – 1) / n.  For n = 1, a(1) = 1.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mulmod_uint64.h"
#include "../utils/pow_seq_uint64.h"

/* binary modular exponentiation: compute (base^exp) mod mod */
static uint64_t modpow_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t acc = base % mod;
    while (exp) {
        if (exp & 1u)
            result = mulmod_uint64(result, acc, mod);
        acc = mulmod_uint64(acc, acc, mod);
        exp >>= 1u;
    }
    return result;
}

/* Sequence value */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* a(1) = 1 by definition */

    uint64_t mod = (uint64_t)n;   /* n fits in uint64_t for the mod operations */

    /* Search for the minimal m ≥ 2 with m^n ≡ 1 (mod n) */
    for (uint64_t m = 2; ; ++m) {
        if (modpow_uint64(m, n, mod) == 1u) {
            /* m found: compute k = (m^n - 1) / n exactly */
            seq_t power = pow_seq_uint64((seq_t)m, n); /* exact m^n  */
            seq_t numer = power - 1;                   /* m^n – 1   */
            seq_t k = numer / (seq_t)n;                /* exact division */
            return k;
        }
    }
}
