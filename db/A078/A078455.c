/*
 * A078455  a(1) = 2. For n>1, a(n) = smallest m such that m == 0 (mod
 *          prime(n)), m + 1 == 0 (mod prime(n+1)) and m-1 == 0 (mod
 *          prime(n-1)).
 * https://oeis.org/A078455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   get_prime, inv_mod_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A078455: for n>1, the smallest m such that
 *   m      ≡ 0   (mod prime(n)),
 *   m + 1  ≡ 0   (mod prime(n+1)),
 *   m – 1  ≡ 0   (mod prime(n‑1)),
 * and a(1) = 2.
 * The computation uses the Chinese remainder theorem.  */

#include <stdint.h>
#include <stdbool.h>

/* Utility that returns the n-th prime (n>=1).  */
#include "../utils/get_prime.h"
#include "../utils/inv_mod_uint64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;   /* base case */

    /* retrieve the three consecutive primes */
    uint32_t p_prev = get_prime(n - 1);  /* prime(n-1) */
    uint32_t p      = get_prime(n);      /* prime(n)   */
    uint32_t p_next = get_prime(n + 1);  /* prime(n+1) */

    /* residues for the congruences, in the order we will combine them */
    uint64_t mods[3]     = {(uint64_t)p, (uint64_t)p_next, (uint64_t)p_prev};
    uint64_t residues[3] = {0ULL,          (uint64_t)p_next - 1ULL,
                            1ULL};

    seq_t result  = 0;
    seq_t mod_prod = 1;               /* product of processed moduli (seq_t) */

    for (int i = 0; i < 3; ++i) {
        uint64_t mod = mods[i];
        uint64_t residue = residues[i];

        /* current remainder of the result modulo mod */
        uint64_t r_mod = (uint64_t)(result % mod);
        /* difference needed to reach the desired residue */
        uint64_t diff = (residue + mod - r_mod) % mod;

        /* inverse of current modulus product modulo mod */
        uint64_t inv = inv_mod_uint64((uint64_t)(mod_prod % mod), mod);
        uint64_t t   = (diff * inv) % mod;

        /* update the result and accumulated modulus product */
        result += (seq_t)t * mod_prod;
        mod_prod *= (seq_t)mod;
    }

    return result;   /* smallest non‑negative solution */
}
