/*
 * A060305  Pisano periods for primes: period of Fibonacci numbers mod
 *          prime(n).
 * https://oeis.org/A060305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3516, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A060305 – Pisano periods of the n‑th prime.
 *
 *  a(n) = Pisano period of Fibonacci numbers modulo prime(n)
 *  For p = 2, 3 or 5 the values are known to be 3, 8 and 20 respectively.
 *  For any other odd prime p the period divides:
 *      p‑1  if 5 is a quadratic residue mod p,
 *      2*(p+1) otherwise.
 *  The minimal divisor of that candidate satisfying F(d)≡0 (mod p) and
 *  F(d+1)≡1 (mod p) is the Pisano period.
 *
 *  The algorithm factors the candidate, then recursively
 *  enumerates all of its divisors, testing each until the smallest
 *  admissible divisor is found.  No temporary array of divisors
 *  is allocated, preventing overflow of the divisor buffer.
 *
 *  All arithmetic stays within 64‑bit unsigned integers; the result
 *  (≤ 2·p+2) is cast to the generic type seq_t.
 *
 *  Expected time per n≤10000 is O(√p + τ(candidate)·log p),
 *  well below the 2‑second limit.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* -------- static prime sieve for the first ~200k numbers ---------- */
static uint64_t primes[200000];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt) return;
    const uint64_t LIM = 200000;
    bool *is_comp = (bool*)malloc((size_t)LIM + 1);
    if (!is_comp) return;                     /* memory failure – unlikely */
    for (uint64_t i = 0; i <= LIM; ++i) is_comp[i] = false;
    for (uint64_t i = 2; i * i <= LIM; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= LIM; j += i) is_comp[j] = true;
        }
    }
    size_t idx = 0;
    for (uint64_t i = 2; i <= LIM; ++i) {
        if (!is_comp[i]) primes[idx++] = i;
    }
    prime_cnt = idx;
    free(is_comp);
}

/* -------- modular arithmetic helpers ---------- */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return ((unsigned __int128)a * b) % mod;
}

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* -------- fast‑doubling Fibonacci modulo mod ---------- */
static void fib_pair_mod(uint64_t n, uint64_t mod,
                         uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) { *fn = 0; *fn1 = 1; return; }
    uint64_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);
    uint64_t two_b = (b << 1) % mod;
    uint64_t term = (two_b + mod - a) % mod;          /* (2*b - a) mod mod */
    uint64_t c = mod_mul(a, term, mod);               /* F(2k)   */
    uint64_t d = (mod_mul(a, a, mod) + mod_mul(b, b, mod)) % mod; /* F(2k+1)*/
    if ((n & 1) == 0) { *fn = c; *fn1 = d; }
    else { *fn = d; *fn1 = (c + d) % mod; }
}

/* -------- prime factorisation of a 64‑bit integer ---------- */
typedef struct { uint64_t prime; int exp; } factor_t;

static size_t factor_uint64(uint64_t n, factor_t *fac)
{
    size_t count = 0;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            fac[count].prime = p; fac[count].exp = e; ++count;
        }
    }
    if (n > 1) { fac[count].prime = n; fac[count].exp = 1; ++count; }
    return count;
}

/* -------- test whether d is a Pisano period modulo p ---------- */
static bool is_period(uint64_t d, uint64_t p)
{
    uint64_t f, fp1;
    fib_pair_mod(d, p, &f, &fp1);
    return (f == 0 && fp1 == 1);
}

/* -------- recursive search of minimal admissible divisor ---------- */
static void search_min(uint64_t d, size_t idx,
                       const factor_t *fac, size_t fcnt,
                       uint64_t p,
                       uint64_t *best, bool *found)
{
    if (idx == fcnt) {
        if (is_period(d, p)) {
            if (!*found || d < *best) { *best = d; *found = true; }
        }
        return;
    }
    const factor_t *f = fac + idx;
    uint64_t curr = 1;
    for (int e = 0; e <= f->exp; ++e) {
        search_min(d * curr, idx + 1, fac, fcnt, p, best, found);
        curr *= f->prime;
    }
}

/* -------- main sequence function ---------- */
seq_t a(idx_t n)
{
    init_primes();
    if (n == 0) return (seq_t)0;              /* undefined below offset */

    uint64_t p = primes[n - 1];                /* nth prime, 1‑based */

    /* special small primes */
    if (p == 2) return (seq_t)3;
    if (p == 3) return (seq_t)8;
    if (p == 5) return (seq_t)20;

    /* decide candidate divisor set */
    uint64_t candidate;
    if (mod_pow(5, (p - 1) >> 1, p) == 1) candidate = p - 1;
    else                                    candidate = 2 * (p + 1);

    /* factor candidate */
    factor_t fac[64];
    size_t fcnt = factor_uint64(candidate, fac);

    /* find minimal divisor that is the Pisano period */
    uint64_t best = 0;
    bool found = false;
    search_min(1, 0, fac, fcnt, p, &best, &found);

    /* In theory we should always find a period; if not, return 0. */
    return (seq_t)(found ? best : 0);
}
