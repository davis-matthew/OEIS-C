/*
 * A106292  Period of the Lucas sequence A000032 mod prime(n).
 * https://oeis.org/A106292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = period of the Lucas sequence A000032 modulo the n‑th
 *            prime
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The period equals the Pisano period of Fibonacci numbers for prime p,
   with the special cases p=2 → period 3, p=5 → period 4. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/powmod_u64.h"

/* ---------- Fast doubling Fibonacci modulo p ----------
   Returns F_n (in *f) and F_{n+1} (in *f_next) modulo the prime p. */
static void fib_pair_mod(uint64_t p, uint64_t n, uint64_t *f, uint64_t *f_next)
{
    if (n == 0) {
        *f = 0;
        *f_next = 1;
        return;
    }
    uint64_t a, b;
    fib_pair_mod(p, n >> 1, &a, &b);

    /* c = F(2k) = F_k * (2*F_{k+1} – F_k) */
    uint64_t t = ((b << 1) % p + p - a) % p;   /* 2*b – a (mod p) */
    uint64_t c = (a * t) % p;

    /* d = F(2k+1) = F_k^2 + F_{k+1}^2 */
    uint64_t d = ((a * a) % p + (b * b) % p) % p;

    if ((n & 1) == 0) {
        *f = c;
        *f_next = d;
    } else {
        *f = d;
        *f_next = (c + d) % p;
    }
}

/* ---------- Generate all divisors from factorisation ----------
   Recursively build divisors; results placed in divs[0..div_idx-1]. */
static void gen_divisors_rec(int idx, const uint64_t *fact, const int *exp,
                             int f_cnt, uint64_t current,
                             uint64_t *divs, int *div_idx)
{
    if (idx == f_cnt) {
        divs[(*div_idx)++] = current;
        return;
    }
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divisors_rec(idx + 1, fact, exp, f_cnt, current, divs, div_idx);
        current *= fact[idx];
    }
}

#define PRIME_LIMIT 400000        /* enough to generate >40000 primes */

static uint64_t primes[PRIME_LIMIT + 1];
static size_t prime_cnt = 0;
static bool primes_inited = false;

/* ---------- Prime sieve initialisation ---------- */
static void init_primes(void)
{
    if (primes_inited) return;
    bool *is_comp = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_comp) return;   /* allocation failure – unlikely for this size */

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[idx++] = i;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    prime_cnt = idx;
    free(is_comp);
    primes_inited = true;
}

/* ---------- Factorisation of an integer n (<≈200 000) ----------
   Returns primes in p_factors[0..f_cnt-1] and exponent in exps[0..]. */
static void factor_candidate(uint64_t n, uint64_t *p_factors, int *exps, int *f_cnt)
{
    int cnt = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t q = primes[i];
        if (temp % q == 0) {
            int e = 0;
            while (temp % q == 0) {
                temp /= q;
                ++e;
            }
            p_factors[cnt] = q;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        p_factors[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }
    *f_cnt = cnt;
}

/* ---------- Compare function for qsort ---------- */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* ---------- Element to compute a(n) ---------- */
seq_t a(idx_t n)
{
    /* Initialise primes once. */
    init_primes();

    if (n == 0) return 0;            /* below the offset – arbitrary */
    if (n > prime_cnt) return 0;     /* out of precomputed range */

    uint64_t p = primes[n - 1];      /* nth prime, 1‑based */

    /* Handle small primes that deviate from the general rule. */
    if (p == 2) return (seq_t)3;
    if (p == 5) return (seq_t)4;

    /* Determines whether 5 is a quadratic residue mod p. */
    uint64_t leg = powmod_u64(5ULL, (p - 1) / 2, p);
    bool residue = (leg == 1);

    /* Candidate period: (p-1) if 5 is a residue, otherwise 2*(p+1). */
    uint64_t cand = residue ? (p - 1) : (2ULL * (p + 1));

    /* Factorise the candidate. */
    uint64_t f_arr[20];
    int exp_arr[20];
    int f_cnt = 0;
    factor_candidate(cand, f_arr, exp_arr, &f_cnt);

    /* Enumerate all divisors. */
    uint64_t divs[2000];
    int div_cnt = 0;
    gen_divisors_rec(0, f_arr, exp_arr, f_cnt, 1, divs, &div_cnt);
    qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* Find the minimal divisor that satisfies the Fibonacci period condition. */
    for (int i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        uint64_t fd, fd1;
        fib_pair_mod(p, d, &fd, &fd1);
        if (fd == 0 && fd1 == 1) {
            return (seq_t)d;
        }
    }

    /* The algorithm guarantees a period exists; unreachable. */
    return (seq_t)0;
}
