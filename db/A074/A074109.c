/*
 * A074109  Smallest n-digit squarefree number.
 * https://oeis.org/A074109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 10^(n-1)+1 unless n-1 ∈ A086982 (in which case return
 *            10^(n-1)+2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/pow10.h"

/* ------------------------------------------------------------------ */
/* Constants and static data                                         */
/* ------------------------------------------------------------------ */
#define MAX_K 10000                 /* precompute bad k up to 10,000  */
#define PRIME_LIMIT 100000          /* sieve limit for primes         */

static bool bad_k[MAX_K + 1] = {0};
static bool init_done = false;

/* ------------------------------------------------------------------ */
/* 128‑bit modular exponentiation: base^exp mod mod_                  */
/* ------------------------------------------------------------------ */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod_) {
    uint64_t res = 1 % mod_;
    uint64_t b   = base % mod_;
    while (exp) {
        if (exp & 1)
            res = (unsigned __int128)res * b % mod_;
        b = (unsigned __int128)b * b % mod_;
        exp >>= 1;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Sieve: fill primes[0..cnt-1] with all primes ≤ PRIME_LIMIT          */
/* ------------------------------------------------------------------ */
static void sieve_primes(int primes[], size_t *cnt) {
    bool is_comp[PRIME_LIMIT + 1] = {false};
    size_t pcnt = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[pcnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT)
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
        }
    }
    *cnt = pcnt;
}

/* ------------------------------------------------------------------ */
/* Trial‑division factorization of n using primes[0..pr_cnt-1]        */
/* Stores distinct prime factors in pf[], returns count via pf_cnt   */
/* ------------------------------------------------------------------ */
static void factorize_uint64(uint64_t n, const int primes[], size_t pr_cnt,
                             int pf[], size_t *pf_cnt) {
    size_t cnt = 0;
    for (size_t i = 0; i < pr_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        int p = primes[i];
        if (n % p == 0) {
            pf[cnt++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1 && cnt < 64) pf[cnt++] = (int)n;
    *pf_cnt = cnt;
}

/* ------------------------------------------------------------------ */
/* Precompute bad_k array: mark all k where 10^k ≡ -1 (mod p^2) for some prime p>5 */
/* ------------------------------------------------------------------ */
static void init_bad_k(void) {
    if (init_done) return;
    init_done = true;

    int primes[PRIME_LIMIT/10 + 1000];   /* enough to hold all primes up to 100000 */
    size_t primes_cnt = 0;
    sieve_primes(primes, &primes_cnt);

    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        int p = primes[pi];
        if (p <= 5) continue;    /* base 10 is 0 (mod 2,5) or 3 (mod 3), no -1 */

        uint64_t p2   = (uint64_t)p * (uint64_t)p;          /* p² fits in 64‑bit here */
        uint64_t phi  = (uint64_t)p * (uint64_t)(p - 1);    /* φ(p²)  */
        uint64_t order = phi;

        /* factor φ(p²) into distinct primes */
        int pf[64];
        size_t pf_cnt;
        factorize_uint64(phi, primes, primes_cnt, pf, &pf_cnt);

        /* Reduce order to its minimal value */
        for (size_t fi = 0; fi < pf_cnt; ++fi) {
            int q = pf[fi];
            while (order % q == 0 &&
                   powmod_uint64(10, order / q, p2) == 1)
                order /= q;
        }

        if ((order & 1ULL) == 0) {                           /* even order */
            uint64_t k = order >> 1;                         /* k = t/2 */
            if (powmod_uint64(10, k, p2) == p2 - 1) {        /* 10^k ≡ -1 (mod p²) */
                for (uint64_t kk = k; kk <= (uint64_t)MAX_K; kk += order)
                    bad_k[(size_t)kk] = true;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public API: sequence entry                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n=0, safety */

    if (n == 1) return (seq_t)2;     /* a(1) = 2 */
    if (n == 2) return (seq_t)10;    /* a(2) = 10 */

    init_bad_k();                    /* ensure bad_k table is ready */

    idx_t k = n - 1;                 /* k = n-1 */

    seq_t base = pow10(k);           /* 10^(n-1) */
    seq_t candidate = base + 1;      /* 10^(n-1)+1 */

    if (k <= MAX_K && bad_k[(size_t)k])
        return candidate + 1;        /* 10^(n-1)+2 when not squarefree */
    else
        return candidate;            /* 10^(n-1)+1 otherwise */
}
