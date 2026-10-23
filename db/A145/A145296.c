/*
 * A145296  Smallest k such that k^2 + 1 is divisible by A002144(n)^3.
 * https://oeis.org/A145296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that k^2 + 1 ≡ 0 (mod p^3), where p =
 *            the n-th prime of the form 4k+1 (A002144). The value is computed
 *            by first finding a square root of –1 modulo p (Tonelli–Shanks)
 *            and then lifting it to modulo p^3 using Hensel’s lemma
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 162, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Helpers: modular exponentiation and inverse over 64‑bit integers   */
/* ------------------------------------------------------------------ */
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    __uint128_t result = 1;
    __uint128_t b = base % mod;
    while (exp) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (uint64_t)result;
}

static uint64_t mod_inverse(uint64_t a, uint64_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = mod, newr = (int64_t)a;
    while (newr != 0) {
        uint64_t q = (uint64_t)(r / newr);
        int64_t tmp = newt;
        newt = t - (__int128)q * newt;
        t = tmp;
        tmp = newr;
        newr = r - (__int128)q * newr;
        r = tmp;
    }
    if (r > 1) return 0;            /* not invertible */
    if (t < 0) t += mod;
    return (uint64_t)t;
}

/* ------------------------------------------------------------------ */
/* Tonelli–Shanks: sqrt(n) modulo an odd prime p (p>2).             */
/* ------------------------------------------------------------------ */
static uint64_t tonelli_shanks(uint64_t n, uint64_t p)
{
    if (n == 0) return 0;
    if (p == 2) return n;
    if (mod_pow(n, (p - 1) / 2, p) != 1) return 0;   /* non‑residue */

    if (p % 4 == 3) return mod_pow(n, (p + 1) / 4, p);

    /* write p-1 = q * 2^s with q odd */
    uint64_t q = p - 1;
    uint64_t s = 0;
    while ((q & 1) == 0) {
        q >>= 1;
        ++s;
    }

    /* find quadratic non‑residue z */
    uint64_t z = 2;
    while (mod_pow(z, (p - 1) / 2, p) != p - 1) ++z;

    uint64_t c = mod_pow(z, q, p);
    uint64_t x = mod_pow(n, (q + 1) / 2, p);
    uint64_t t = mod_pow(n, q, p);
    uint64_t m = s;

    while (t != 1) {
        uint64_t i = 1;
        uint64_t t2i = (t * t) % p;
        while (t2i != 1) {
            t2i = (t2i * t2i) % p;
            ++i;
            if (i == m) return 0;
        }
        uint64_t b = mod_pow(c, ((uint64_t)1) << (m - i - 1), p);
        x = (x * b) % p;
        t = (t * (b * b % p)) % p;
        c = (b * b) % p;
        m = i;
    }
    return x;
}

/* ------------------------------------------------------------------ */
/* Sieve of Eratosthenes that keeps primes ≡ 1 (mod 4)                */
/* ------------------------------------------------------------------ */
static const idx_t SIEVE_LIMIT = 2000000ULL;   /* covers > 10000 terms */
static uint64_t *primes1 = NULL;
static size_t primes1_cap = 0;
static size_t primes1_cnt = 0;
static bool sieve_done = false;

static void init_sieve(void)
{
    if (sieve_done) return;
    sieve_done = true;

    char *is_comp = (char *)malloc((SIEVE_LIMIT + 1) * sizeof(char));
    memset(is_comp, 0, SIEVE_LIMIT + 1);
    is_comp[0] = is_comp[1] = 1;
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    primes1_cap = SIEVE_LIMIT / 2 + 10;
    primes1 = (uint64_t *)malloc(primes1_cap * sizeof(uint64_t));
    primes1_cnt = 0;
    for (uint64_t i = 5; i <= SIEVE_LIMIT; i += 4) {
        if (!is_comp[i]) {
            if (primes1_cnt < primes1_cap)
                primes1[primes1_cnt++] = i;
        }
    }
    free(is_comp);
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */

    init_sieve();
    if (n > primes1_cnt) return (seq_t)0;   /* safety */

    uint64_t p = primes1[n - 1];               /* p = A002144(n) */
    uint64_t r = tonelli_shanks(p - 1, p);     /* sqrt(-1) mod p */

    uint64_t m = p;         /* current modulus m = p^k (k starts at 1) */
    for (int step = 0; step < 2; ++step) {    /* two lifts -> p^3 */
        __uint128_t f = (__uint128_t)r * r + 1;
        uint64_t kdiv = (uint64_t)(f / m);    /* f = k*m */
        uint64_t kmodp = kdiv % p;
        uint64_t inv = mod_inverse((2 * r) % p, p);
        uint64_t t = (p - kmodp) % p;         /* t = -k mod p */
        t = (t * inv) % p;
        r += t * m;                           /* new root */
        m *= p;                               /* modulus becomes p^(k+1) */
    }

    uint64_t other = m - r;
    uint64_t best = (r <= other) ? r : other;
    return (seq_t)best;
}
