/*
 * A162961  a(n) is the smallest k such that there exists a permutation b_0,
 *          b_1, ... of the maximal prime powers dividing n satisfying b_i |
 *          k+i for all i.
 * https://oeis.org/A162961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A162961:  a(n) is the smallest k such that
   there exists a permutation of the maximal prime powers of n with each
   power dividing a different k + offset.  For a prime power a(n)=n. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

/* Modular inverse of m modulo p^e (a prime power).  Because m and
   p^e are coprime, the inverse exists and can be computed as
   m^(phi-1) mod p^e where phi = p^e - p^(e-1).  The exponent is
   small (≤10^4). */
static seq_t modinv_prime_power(seq_t m, seq_t mod_i, uint64_t p)
{
    /* phi = mod_i - mod_i / p */
    seq_t phi = mod_i - (mod_i / p);
    /* phi fits in 64 bits for all inputs (mod_i <= 10000) */
    uint64_t exp = (uint64_t)(phi - (seq_t)1);
    return mod_pow(m, exp, mod_i);
}

/* Recursive search over all permutations of the prime powers.
   Arguments:
     mods   - array of the prime powers p^e
     primes - array of the corresponding primes p
     P      - number of distinct primes
     depth  - current offset index (0 .. P-1)
     used   - flag array indicating which mods have been used
     a_res  - current residue modulo current modulus m
     m_cur  - current modulus (product of used mods)
     best   - pointer to minimal k found so far (0 if not set yet) */
static void dfs(const seq_t *mods, const uint64_t *primes, int P, int depth,
                int *used, seq_t a_res, seq_t m_cur, seq_t *best)
{
    if (depth == P) {
        seq_t cand = a_res % m_cur;
        if (cand == 0) cand = m_cur;        /* ensure positive */
        if (*best == 0 || cand < *best) *best = cand;
        return;
    }

    for (int i = 0; i < P; ++i) {
        if (!used[i]) {
            used[i] = 1;
            const seq_t mod_i = mods[i];
            const uint64_t prime_i = primes[i];

            seq_t offset = (seq_t)depth;          /* offset = depth */
            seq_t rem = offset % mod_i;
            seq_t r = (rem == 0) ? (seq_t)0 : mod_i - rem;   /* desired remainder */

            seq_t a_mod = a_res % mod_i;
            seq_t delta = (r + mod_i - a_mod) % mod_i;     /* (r - a_res) mod mod_i */

            seq_t inv_m = modinv_prime_power(m_cur, mod_i, prime_i);
            seq_t t = (delta * inv_m) % mod_i;

            seq_t new_a = a_res + m_cur * t;
            seq_t new_m = m_cur * mod_i;

            dfs(mods, primes, P, depth + 1, used, new_a, new_m, best);
            used[i] = 0;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Factor n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* For a prime power, a(n) = n. */
    if (cnt == 1) return (seq_t)n;

    /* Build the list of prime powers. */
    seq_t mods[64];
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t power = 1;
        for (uint64_t e = 0; e < exps[i]; ++e) power *= p;
        mods[i] = power;
    }

    int used[64] = {0};
    seq_t best = 0;   /* 0 means not set yet */
    dfs(mods, primes, (int)cnt, 0, used, (seq_t)0, (seq_t)1, &best);
    return best;
}
