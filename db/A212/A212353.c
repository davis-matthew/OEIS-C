/*
 * A212353  a(n) is the smallest positive solutions of the congruence x^2 +
 *          (x+1)^2 == 0 (mod prime), where prime = A002144(n) (Pythagorean
 *          primes).
 * https://oeis.org/A212353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 6345, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the smallest positive solution of
   x^2 + (x+1)^2 ≡ 0 (mod p_n), where p_n is the n‑th prime ≡ 1 (mod 4).
   The companion solution is (p_n − 1) − a(n).  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- small modular helpers ---------- */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = mod_mul64(res, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* ---------- Jacobi symbol for odd modulus p ---------- */
static int jacobi_symbol(uint64_t a, uint64_t n)
{
    int result = 1;
    a %= n;
    while (a && n > 1) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n8 = n & 7U;
            if (n8 == 3 || n8 == 5) result = -result;
        }
        uint64_t tmp = a; a = n; n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3) result = -result;
        a %= n;
    }
    return (n == 1) ? result : 0;
}

/* ---------- Tonelli–Shanks for sqrt(-1) mod p ---------- */
static uint64_t sqrt_minus_one_mod(uint64_t p)
{
    uint64_t a = p - 1;          /* -1 mod p */
    uint64_t Q = a;
    int S = 0;
    while ((Q & 1) == 0) { Q >>= 1; ++S; }

    /* find a quadratic non‑residue z */
    uint64_t z = 2;
    while (jacobi_symbol(z, p) != -1) ++z;

    uint64_t c = mod_pow64(z, Q, p);
    uint64_t t = mod_pow64(a, Q, p);
    uint64_t r = mod_pow64(a, (Q + 1) >> 1, p);

    while (t != 1) {
        int i = 1;
        uint64_t t2 = mod_mul64(t, t, p);
        while (t2 != 1) {
            t2 = mod_mul64(t2, t2, p);
            ++i;
        }
        uint64_t exp = 1ULL << (S - i - 1);
        uint64_t b = mod_pow64(c, exp, p);
        r = mod_mul64(r, b, p);
        t = mod_mul64(mod_mul64(t, b, p), b, p);
        c = mod_mul64(b, b, p);
        S = i;
    }
    return r;
}

/* ---------- sieve for primes ≡ 1 (mod 4) ---------- */
static uint64_t *primes_mod4 = NULL;
static size_t primes_mod4_cnt = 0;
#define PRIMES_LIMIT 500000   /* sufficient for n ≤ 10000 */

/* build the list once, lazily */
static void init_primes(void)
{
    if (primes_mod4) return;  /* already built */

    bool *is_composite = (bool*)calloc(PRIMES_LIMIT + 1, sizeof(bool));
    for (size_t i = 2; i * i <= PRIMES_LIMIT; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= PRIMES_LIMIT; j += i)
                is_composite[j] = true;

    /* count primes 1 mod 4 */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i)
        if (!is_composite[i] && (i % 4 == 1)) ++cnt;

    primes_mod4 = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i)
        if (!is_composite[i] && (i % 4 == 1))
            primes_mod4[idx++] = (uint64_t)i;

    primes_mod4_cnt = cnt;
    free(is_composite);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, but defined to return 0 */
    init_primes();
    if ((size_t)n > primes_mod4_cnt) return 0;  /* safety guard */

    uint64_t p = primes_mod4[(size_t)n - 1];   /* the prime p_n */

    uint64_t r = sqrt_minus_one_mod(p);        /* sqrt(-1) mod p */

    uint64_t inv2 = (p + 1) >> 1;              /* inverse of 2 mod p */

    /* two incongruent solutions */
    uint64_t x1 = mod_mul64((r + p - 1) % p, inv2, p);
    uint64_t x2 = mod_mul64((p - r - 1) % p, inv2, p);

    /* return the smaller positive solution */
    uint64_t ans = (x1 < x2) ? x1 : x2;
    return (seq_t)ans;
}
