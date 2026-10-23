/*
 * A131190  Numbers n>=0 such that d(n) = (n^1 + 1) (n^2 + 2) ... (n^25 + 25)
 *          / 25! is nonintegral.
 * https://oeis.org/A131190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonintegral numbers n≥0 for which
   d(n) = ∏_{k=1}^{25} (n^k + k) / 25! is not an integer.
   The indices are 1‑based: a(1)=2, a(2)=7, …  */
#include <stdint.h>
#include <stdbool.h>

/* Helper: modular exponentiation, all data fit in 64 bits */
static inline uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        exp >>= 1;
        if (exp)
            base = (base * base) % mod;
    }
    return result;
}

/* Test whether n is a nonintegral member of the sequence */
static inline bool is_nonintegral(idx_t n)
{
    /* primes dividing 25! and required exponents */
    static const idx_t primes[9]    = {2, 3, 5, 7, 11, 13, 17, 19, 23};
    static const idx_t req_exps[9]  = {22, 10, 6, 3, 2, 1, 1, 1, 1};
    /* precomputed prime powers used as modulus */
    static const uint64_t mods[9]   = {4194304ULL, 59049ULL, 15625ULL,
                                      343ULL, 121ULL, 13ULL, 17ULL, 19ULL, 23ULL};

    uint64_t total[9] = {0};

    for (uint64_t k = 1; k <= 25; ++k) {
        for (int i = 0; i < 9; ++i) {
            uint64_t mod   = mods[i];
            uint64_t val   = (pow_mod_uint64(n, k, mod) + k) % mod;
            uint64_t exp   = 0;
            uint64_t p     = primes[i];
            uint64_t req   = req_exps[i];
            while (exp < req && (val % p) == 0) {
                ++exp;
                val /= p;
            }
            total[i] += exp;
            if (total[i] > req) total[i] = req;   /* cap to required */
        }
        /* Early exit: if all required exponents are already met,
           the product is integral and n is not in the sequence */
        bool all_met = true;
        for (int i = 0; i < 9; ++i)
            if (total[i] < req_exps[i]) { all_met = false; break; }
        if (all_met) return false;
    }

    /* After all terms, check if any prime exponent is missing */
    for (int i = 0; i < 9; ++i)
        if (total[i] < req_exps[i]) return true;
    return false;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    idx_t count     = 0;
    idx_t candidate = 0;

    while (count < n) {
        ++candidate;                          /* test next n */
        if (is_nonintegral(candidate))
            ++count;
    }

    return (seq_t)candidate;
}
