/*
 * A023135  Number of cycles of function f(x) = 3x mod n.
 * https://oeis.org/A023135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|m} phi(d)/ord(3,d), where m is n with all factors
 *            3 removed. Implementation uses the equivalent expression with
 *            modular powers and gcd
 * Range:     u8 n <= 848, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, gcd_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/gcd_u64.h"

/* Euclidean gcd for 64‑bit unsigned integers. */

/* Simple trial‑division factorisation into arrays of primes and exponents. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            pr[*cnt] = p;
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            exp[*cnt] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        pr[*cnt] = n;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

/* ERF: a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; undefined for n=0 */

    uint64_t temp = (uint64_t)n;

    /* Remove all factors of 3. */
    while (temp % 3 == 0) temp /= 3;

    seq_t m = (seq_t)temp;
    if (m == (seq_t)1) return (seq_t)1;   /* powers of 3: only one cycle */

    /* ---------- Compute ord(3,m) ---------- */
    uint64_t pr[32], exp[32], cnt;
    factor_uint64(temp, pr, exp, &cnt);

    /* Carmichael function λ(m). */
    uint64_t lambda = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = exp[i];
        uint64_t lam_part;
        if (p == 2) {
            if (e == 1) lam_part = 1;
            else if (e == 2) lam_part = 2;
            else lam_part = 1ULL << (e - 2);
        } else {
            lam_part = 1;
            for (uint64_t k = 1; k < e; ++k) lam_part *= p;
            lam_part *= (p - 1);
        }
        uint64_t g = gcd_u64(lambda, lam_part);
        lambda = (lambda / g) * lam_part;
    }

    /* Reduce λ(m) to the actual multiplicative order of 3 modulo m. */
    factor_uint64(lambda, pr, exp, &cnt);
    uint64_t ord = lambda;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = pr[i];
        while (ord % q == 0) {
            uint64_t candidate = ord / q;
            if ((mod_pow((seq_t)3, candidate, m) == (seq_t)1))
                ord = candidate;
            else
                break;
        }
    }

    /* ---------- Sum of gcds ---------- */
    seq_t pow_val = (seq_t)1;
    seq_t sum = (seq_t)0;
    for (uint64_t j = 0; j < ord; ++j) {
        seq_t diff = pow_val - (seq_t)1;          /* pow_val is always ≥1 */
        seq_t g = gcd(diff, m);
        sum += g;
        pow_val = (pow_val * (seq_t)3) % m;
    }

    /* Final division: sum is a multiple of ord. */
    return sum / (seq_t)ord;
}
