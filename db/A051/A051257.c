/*
 * A051257  Numbers formed from binomial coefficients (mod 2+k) interpreted as
 *          digits in factorial base.
 * https://oeis.org/A051257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, base
 * Formula:   a(n) = Sum_{k=0..n} (k+1)! * (C(n, k) mod (k+2))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 195 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Generate the list of primes up to limit <= 10000.
   The array `primes` must have space for at least 11000 entries
   and `cnt` will contain the number of primes found. */
static void generate_primes_upto(idx_t limit, idx_t *primes, size_t *cnt) {
    static unsigned char sieve[10001] = {0};
    idx_t i, j;
    for (i = 0; i <= limit; ++i) sieve[i] = 0;
    *cnt = 0;
    for (i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            if (*cnt < 11000) primes[(*cnt)++] = i;
            if (i * i <= limit) {
                for (j = i * i; j <= limit; j += i) sieve[j] = 1;
            }
        }
    }
}

/* auxiliary: exponent of prime p in m! */
static inline idx_t exp_factorial(idx_t m, idx_t p) {
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

/* auxiliary: modular power, base, exp, mod are < 2^64 */
static inline uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    while (exp) {
        if (exp & 1ULL) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* primes up to current n, cached */
    static idx_t primes[11000];
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;
    if (n > primes_limit) {
        generate_primes_upto(n, primes, &primes_cnt);
        primes_limit = n;
    }

    /* pre‑compute exponents of n! for each prime */
    static idx_t exp_n[11000];
    for (size_t i = 0; i < primes_cnt; ++i) {
        exp_n[i] = exp_factorial(n, primes[i]);
    }

    seq_t sum   = 0;
    seq_t fact  = 1;          /* will store (k+1)! during the loop */

    for (idx_t k = 0; k <= n; ++k) {
        /* modulus for this k */
        idx_t mod = k + 2;
        uint64_t bin_mod = 0;
        if (mod > 1) {
            uint64_t res = 1 % mod;
            for (size_t i = 0; i < primes_cnt; ++i) {
                idx_t p   = primes[i];
                idx_t e   = exp_n[i]
                            - exp_factorial(k,       p)
                            - exp_factorial(n - k,   p);
                if (e > 0) {
                    uint64_t r = pow_mod_uint64(p % mod, e, mod);
                    res = (res * r) % mod;
                }
            }
            bin_mod = res;
        }   /* if mod==1, bin_mod stays 0 */

        /* add term (k+1)! * bin_mod */
        sum += fact * (seq_t)bin_mod;

        /* update factorial for next k: (k+2)! */
        fact *= (seq_t)(k + 2);
    }

    return sum;
}
