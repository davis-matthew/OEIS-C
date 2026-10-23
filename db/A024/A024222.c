/*
 * A024222  Number of shuffles (perfect faro shuffles with cut) required to
 *          return a deck of size n to its original order.
 * https://oeis.org/A024222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n=1, 1 for n=2 a(n) = multiplicative order of 2
 *            modulo n if n is odd (>1) a(n) = multiplicative order of 2
 *            modulo (n-1) if n is even (>2)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard integer types */
#include <stdint.h>
#include <stddef.h>

/* fast modular exponentiation for uint64_t */
static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t a   = base % mod;
    while (exp > 0) {
        if (exp & 1ULL)
            res = (uint64_t)((res * a) % mod);
        a = (uint64_t)((a * a) % mod);
        exp >>= 1ULL;
    }
    return res;
}

/* simple trial division factorisation, n <= 65535 for this problem */
static void factor_uint64(uint64_t n, uint64_t primes[32], uint64_t exps[32], size_t *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            size_t c = 0;
            while (n % p == 0) {
                n /= p;
                ++c;
            }
            primes[(*cnt)] = p;
            exps[(*cnt)]   = c;
            ++(*cnt);
        }
    }
    if (n > 1) {
        primes[(*cnt)] = n;
        exps[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* public function required by the template */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* n=1 */
    if (n == 2) return (seq_t)1;   /* n=2 */

    /* m is the modulus for the multiplicative order of 2 */
    uint64_t m = (n % 2 == 0) ? (uint64_t)(n - 1) : (uint64_t)n;

    /* compute Euler's totient φ(m) */
    uint64_t prime_m[32], exp_m[32];
    size_t cnt_m = 0;
    factor_uint64(m, prime_m, exp_m, &cnt_m);

    uint64_t phi = m;
    for (size_t i = 0; i < cnt_m; ++i) {
        uint64_t p = prime_m[i];
        phi = phi / p * (p - 1);
    }

    /* factor φ(m) to find its prime factors */
    uint64_t prime_phi[32], exp_phi[32];
    size_t cnt_phi = 0;
    factor_uint64(phi, prime_phi, exp_phi, &cnt_phi);

    /* compute the multiplicative order of 2 modulo m */
    uint64_t order = phi;
    for (size_t i = 0; i < cnt_phi; ++i) {
        uint64_t p = prime_phi[i];
        while (order % p == 0 &&
               modpow_uint64(2ULL, order / p, m) == 1ULL)
            order /= p;
    }

    return (seq_t)order;
}
