/*
 * A076947  Smallest k > 0 such that nk+1 is a cube.
 * https://oeis.org/A076947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that n·k + 1 is a cube
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   cube_roots_mod_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/cube_roots_mod_prime_power.h"

/* Extended Euclidean algorithm for 64‑bit integers.
   Returns gcd(a,b) and writes the Bézout coefficients into x and y. */
static uint64_t egcd64(uint64_t a, uint64_t b, int64_t *x, int64_t *y)
{
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int64_t x1, y1;
    uint64_t g = egcd64(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (int64_t)(a / b) * y1;
    return g;
}

/* Modular inverse of a modulo mod (a and mod are coprime). */
static uint64_t mul_mod_inv(uint64_t a, uint64_t mod)
{
    int64_t x, y;
    uint64_t g = egcd64(a, mod, &x, &y);
    if (g != 1) return 0;            /* inverse does not exist */
    int64_t inv = x % (int64_t)mod;
    if (inv < 0) inv += (int64_t)mod;
    return (uint64_t)inv;
}

/* Compute p^e (e >= 1).  Overflow is not expected for the test data. */
static uint64_t pow_uint64(uint64_t p, unsigned e)
{
    uint64_t r = 1;
    while (e--) r *= p;
    return r;
}

/* Trial division factorisation of n.
   Out array primes[32] holds the distinct primes,
   exps[32] holds their exponents, and *cnt gets the count. */
static void factor_uint64(uint64_t n, uint64_t *primes, int *exps, int *cnt)
{
    int i = 0;
    uint64_t p = 2;
    while (p * p <= n) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[i] = p;
            exps[i] = e;
            ++i;
        }
        p = (p == 2) ? 3 : p + 2;
    }
    if (n > 1) {
        primes[i] = n;
        exps[i] = 1;
        ++i;
    }
    *cnt = i;
}

/* Combine two congruences:
        x ≡ a (mod M)
        x ≡ b (mod m)
   where M and m are coprime.
   The result x is returned modulo M*m. */
static uint64_t crt_combine(uint64_t a, uint64_t M, uint64_t b, uint64_t m)
{
    uint64_t inv = mul_mod_inv(M % m, m);     /* M^{-1} mod m */
    uint64_t diff = (b >= a % m) ? b - (a % m) : m - ((a % m) - b);
    diff = (__uint128_t)diff * inv % m;
    __uint128_t add = (__uint128_t)M * diff;
    return a + (uint64_t)add;                 /* a + M*diff   */
}

/* Find the minimal residue m ( > 1 ) such that m^3 ≡ 1 (mod n). */
static uint64_t minimal_cube_root_mod_n(uint64_t n)
{
    if (n == 1) return 2;      /* 2^3 = 8 ≡ 1 (mod 1); k = (8-1)/1 = 7 */

    /* Factorise */
    uint64_t primes[32];
    int exps[32], pf_cnt;
    factor_uint64(n, primes, exps, &pf_cnt);

    /* Arrays for current residues (up to 3^ω(n)) */
    uint64_t residues[5120];
    int res_cnt = 0;
    uint64_t cur_mod = 1;

    for (int i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        uint64_t mod = pow_uint64(p, e);

        /* Roots modulo mod */
        uint64_t roots[32];
        int roots_cnt;
        cube_roots_mod_prime_power(mod, roots, &roots_cnt);

        if (i == 0) {
            /* first factor: start the list */
            for (int j = 0; j < roots_cnt; ++j) {
                residues[res_cnt++] = roots[j];
            }
            cur_mod = mod;
        } else {
            /* combine with previous list */
            uint64_t new_res[5120];
            int new_cnt = 0;
            for (int a = 0; a < res_cnt; ++a) {
                for (int b = 0; b < roots_cnt; ++b) {
                    uint64_t x = crt_combine(residues[a], cur_mod, roots[b], mod);
                    new_res[new_cnt++] = x % (cur_mod * mod);
                }
            }
            cur_mod *= mod;
            for (int k = 0; k < new_cnt; ++k) residues[k] = new_res[k];
            res_cnt = new_cnt;
        }
    }

    /* Find minimal residue > 1 */
    uint64_t best = n + 1;        /* fallback: m = n + 1 */
    for (int i = 0; i < res_cnt; ++i) {
        uint64_t r = residues[i] % n;
        if (r > 1 && r < best) best = r;
    }
    /* If no r > 1, best stays n+1 */
    return best == n + 1 ? n + 1 : best;
}

seq_t a(idx_t n)
{
    uint64_t m = minimal_cube_root_mod_n((uint64_t)n);   /* small enough for 64‑bit */
    seq_t mm = (seq_t)m;
    seq_t cube = mm * mm * mm;      /* m³, fits in seq_t */
    seq_t k = (cube - (seq_t)1) / (seq_t)n;
    return k;
}
