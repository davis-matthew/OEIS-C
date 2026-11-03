/*
 * A120947  a(n) = smallest m such that n-th prime divides Pell(m).
 * https://oeis.org/A120947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9822 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A120947: a(n) = smallest m such that the n‑th prime divides Pell(m) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Legendre symbol (2|p): 1 if p≡±1 (mod 8), –1 otherwise */
static int legendre2(uint32_t p)
{
    switch (p & 7u) {                 /* p mod 8 */
        case 1u: case 7u: return 1;
        default:               return -1;
    }
}

/* fast exponentiation of the Pell matrix [[2,1],[1,0]] modulo mod */
static uint64_t pell_mod_u64(uint64_t mod, idx_t n)
{
    if (mod <= 1u)
        return 0u;
    if (n == 0u)
        return 0u;
    if (n == 1u)
        return 1 % mod;

    /* M = [[2,1],[1,0]] */
    uint64_t a = 2 % mod, b = 1 % mod, c = 1 % mod, d = 0 % mod;
    /* R = identity */
    uint64_t ra = 1 % mod, rb = 0, rc = 0, rd = 1 % mod;

    while (n != 0u) {
        if (n & 1u) {
            /* R = R * M */
            uint64_t na = (ra * a + rb * c) % mod;
            uint64_t nb = (ra * b + rb * d) % mod;
            uint64_t nc = (rc * a + rd * c) % mod;
            uint64_t nd = (rc * b + rd * d) % mod;
            ra = na; rb = nb; rc = nc; rd = nd;
        }
        /* M = M * M */
        uint64_t na = (a * a + b * c) % mod;
        uint64_t nb = (a * b + b * d) % mod;
        uint64_t nc = (c * a + d * c) % mod;
        uint64_t nd = (c * b + d * d) % mod;
        a = na; b = nb; c = nc; d = nd;
        n >>= 1u;
    }
    return rc % mod;                     /* entry (0,1) of the final matrix */
}

/* ---------- Prime cache utilities ---------- */
static uint32_t *prime_cache = NULL;  /* array of primes, 1‑based (index-1 in array) */
static size_t   prime_cnt   = 0;      /* number of primes cached */

/* Build a small sieve up to LIMIT and cache the primes */
static void init_prime_cache(void)
{
    if (prime_cache != NULL)
        return;                    /* already built */

    /* Limit big enough for at least the 10 000th prime (~104 729) */
    const size_t LIMIT = 200000;  /* safety margin */
    bool *is_prime = (bool *)malloc(LIMIT + 1);
    memset(is_prime, 1, LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;

    /* Count primes and allocate array */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++prime_cnt;

    prime_cache = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            prime_cache[idx++] = (uint32_t)i;
    free(is_prime);
}

/* Return the k‑th prime (k>=1) */
static uint32_t get_prime_at(idx_t k)   /* 1‑based */
{
    init_prime_cache();
    if (k == 0 || (size_t)k > prime_cnt)
        return 0;                      /* out of range, caller must not ask for it */
    return prime_cache[k - 1];
}

/* ---------- Euler/Legendre utility ---------- */

/* ---------- Pell number modulo small modulus ---------- */

/* ---------- Divisors of an integer ---------- */

/* Fill array divs (size at least 2048) with all divisors of n, return count */
static void get_divisors(idx_t n, idx_t *divs, size_t *cnt)
{
    size_t pos = 0;
    divs[pos++] = 1;

    idx_t m = n;
    for (idx_t f = 2; f * f <= m; ++f) {
        if (m % f == 0) {
            size_t exp = 0;
            while (m % f == 0) {
                m /= f;
                ++exp;
            }
            size_t cur = pos;
            idx_t pow_f = 1;
            for (size_t e = 1; e <= exp; ++e) {
                pow_f *= f;
                for (size_t i = 0; i < cur; ++i)
                    divs[pos++] = divs[i] * pow_f;
            }
        }
    }
    if (m > 1) {                  /* remaining prime factor */
        size_t cur = pos;
        for (size_t i = 0; i < cur; ++i)
            divs[pos++] = divs[i] * m;
    }
    *cnt = pos;
}

static int cmp_idx(const void *a, const void *b)
{
    idx_t v1 = *(const idx_t *)a;
    idx_t v2 = *(const idx_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1u)
        return (seq_t)0;          /* undefined for n<1 */

    uint32_t p = get_prime_at(n);     /* n-th prime */

    /* Special case p==2 : Pell(2)=2 */
    if (p == 2u)
        return (seq_t)2;

    int leg = legendre2(p);
    /* T is the order bound for the multiplicative order of -3-2*sqrt(2) */
    idx_t T = (leg == 1) ? (idx_t)(p - 1u) : (idx_t)(2ULL * (p + 1u));

    /* Compute all divisors of T and test in ascending order */
    static idx_t divs[2048];
    size_t divcnt = 0;
    get_divisors(T, divs, &divcnt);
    qsort(divs, divcnt, sizeof(idx_t), cmp_idx);

    for (size_t i = 0; i < divcnt; ++i) {
        idx_t d = divs[i];
        if (pell_mod_u64((uint64_t)p, d) == 0u)
            return (seq_t)d;
    }

    /* Should never happen for a prime p */
    return (seq_t)0;
}
