/*
 * A229466  Numbers k such that the period of Fibonacci numbers mod k is
 *          3*(k+10).
 * https://oeis.org/A229466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = k such that the Pisano period π(k) of the Fibonacci
   sequence modulo k equals 3·(k+10).  The function generates the terms
   by searching multiples of 10, computing the Pisano period of each k,
   and collecting those that satisfy the equality. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------------ */
/* Prime sieve (static, built once)                                    */
/* ------------------------------------------------------------------ */
static uint64_t *PRIMES      = NULL;
static size_t   PRIME_CNT    = 0;
static size_t   PRIME_CAP    = 0;
static seq_t   *PRIME_PERIOD = NULL;   /* cached period of each prime */

static void build_primes(void)
{
    if (PRIMES)
        return;                    /* already built */

    /* Limit chosen large enough for k up to a few million */
    const uint64_t LIMIT = 2000000ULL;   /* ~2 million, 2 MB for bool array */

    PRIMES  = (uint64_t *)malloc(LIMIT * sizeof(uint64_t));
    PRIME_CAP = LIMIT;   /* we will never re‑alloc for this limit */
    if (!PRIMES) exit(1);

    char *is_comp = (char *)calloc(LIMIT + 1, sizeof(char));
    if (!is_comp) { free(PRIMES); exit(1); }

    for (uint64_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;

    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) {
            PRIMES[PRIME_CNT++] = i;
        }

    free(is_comp);

    /* Allocate period cache for each prime */
    PRIME_PERIOD = (seq_t *)calloc(PRIME_CNT, sizeof(seq_t));
    if (!PRIME_PERIOD) exit(1);
}

/* ------------------------------------------------------------------ */
/* Helper: binary search for prime index                            */
/* ------------------------------------------------------------------ */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = PRIME_CNT;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (PRIMES[mid] == p)
            return mid;
        if (PRIMES[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (size_t)-1;               /* should never happen */
}

/* ------------------------------------------------------------------ */
/* Fast doubling Fibonacci modulo m                                 */
/* ------------------------------------------------------------------ */
static void fib_mod_pair(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    uint64_t a, b;
    fib_mod_pair(n >> 1, mod, &a, &b);
    uint64_t c = (a * ((2 * b % mod + mod - a) % mod)) % mod;   /* F(2k)   */
    uint64_t d = ((a * a) % mod + (b * b) % mod) % mod;         /* F(2k+1) */
    if ((n & 1ULL) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

/* ------------------------------------------------------------------ */
/* Check if d is a Pisano period for prime p                          */
/* ------------------------------------------------------------------ */
static int period_of_prime_test(uint64_t p, uint64_t d)
{
    uint64_t f, f1;
    fib_mod_pair(d, p, &f, &f1);
    return f == 0 && f1 == 1;
}

/* ------------------------------------------------------------------ */
/* Factorisation of 64‑bit number into prime factors                 */
/* ------------------------------------------------------------------ */
#define MAX_FACTORS 64
static void factor_uint64(uint64_t n,
                          uint64_t *factors,
                          int     *exps,
                          int     *pf_cnt)
{
    *pf_cnt = 0;
    for (size_t i = 0; i < PRIME_CNT && PRIMES[i] * PRIMES[i] <= n; ++i) {
        uint64_t q = PRIMES[i];
        if (n % q == 0) {
            int cnt = 0;
            while (n % q == 0) {
                n /= q;
                ++cnt;
            }
            factors[*pf_cnt]   = q;
            exps[*pf_cnt]      = cnt;
            ++(*pf_cnt);
        }
    }
    if (n > 1) {                     /* remaining prime factor */
        factors[*pf_cnt]   = n;
        exps[*pf_cnt]      = 1;
        ++(*pf_cnt);
    }
}

/* ------------------------------------------------------------------ */
/* Recursive generation of all divisors of a number                   */
/* ------------------------------------------------------------------ */
#define MAX_DIVISORS 10000
static void gen_divisors_recursive(const uint64_t *factors,
                                   const int     *exps,
                                   int            pf_cnt,
                                   int            depth,
                                   uint64_t       cur,
                                   uint64_t *divs,
                                   int     *div_cnt)
{
    if (depth == pf_cnt) {
        divs[(*div_cnt)++] = cur;
        return;
    }
    uint64_t p = factors[depth];
    uint64_t val = 1;
    for (int e = 0; e <= exps[depth]; ++e) {
        gen_divisors_recursive(factors, exps, pf_cnt, depth + 1,
                               cur * val, divs, div_cnt);
        val *= p;
    }
}

/* Simple insertion sort for divisor array                           */
static void sort_divisors(uint64_t *divs, int div_cnt)
{
    for (int i = 1; i < div_cnt; ++i) {
        uint64_t key = divs[i];
        int j = i - 1;
        while (j >= 0 && divs[j] > key) {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }
}

/* ------------------------------------------------------------------ */
/* Period of a prime p (p ≠ 2,5)                                    */
/* ------------------------------------------------------------------ */
static seq_t prime_period_cached(uint64_t p)
{
    if (p == 2) return (seq_t)3;
    if (p == 5) return (seq_t)20;

    size_t idx = prime_index(p);
    if (idx == (size_t)-1) {
        /* p is larger than sieve limit – compute on the fly */
        /* (unlikely for required range) */
        sort_divisors; /* harmless, compile only */
    }

    seq_t p_period = PRIME_PERIOD[idx];
    if (p_period != 0)
        return p_period;

    /* bound for the Pisano period of p */
    uint64_t bound = (p % 5 == 1 || p % 5 == 4) ? (p - 1) : 2 * (p + 1);

    uint64_t f[ MAX_FACTORS ];
    int     e[ MAX_FACTORS ];
    int     cnt;
    factor_uint64(bound, f, e, &cnt);

    uint64_t divs[ MAX_DIVISORS ];
    int div_cnt = 0;
    gen_divisors_recursive(f, e, cnt, 0, 1ULL, divs, &div_cnt);
    sort_divisors(divs, div_cnt);

    seq_t res = (seq_t)bound;
    for (int i = 0; i < div_cnt; ++i) {
        if (period_of_prime_test(p, divs[i])) {
            res = (seq_t)divs[i];
            break;
        }
    }

    PRIME_PERIOD[idx] = res;
    return res;
}

/* ------------------------------------------------------------------ */
/* Period of a prime power p^e                                        */
/* ------------------------------------------------------------------ */
static seq_t period_prime_power(uint64_t p, int e)
{
    if (p == 2) {
        if (e == 1) return (seq_t)3;
        if (e == 2) return (seq_t)6;
        seq_t r = (seq_t)3;
        for (int i = 0; i < e - 1; ++i)
            r *= (seq_t)2;
        return r;
    }

    if (p == 5) {
        if (e == 1) return (seq_t)20;
        seq_t r = (seq_t)20;
        for (int i = 0; i < e - 1; ++i)
            r *= (seq_t)5;
        return r;
    }

    seq_t per_p = prime_period_cached(p);
    seq_t r = per_p;
    for (int i = 1; i < e; ++i)
        r *= (seq_t)p;
    return r;
}

/* ------------------------------------------------------------------ */
/* Pisano period of k                                                  */
/* ------------------------------------------------------------------ */
static seq_t pisano_period(uint64_t k)
{
    uint64_t f[ MAX_FACTORS ];
    int     e[ MAX_FACTORS ];
    int     cnt;
    factor_uint64(k, f, e, &cnt);

    seq_t lcm = (seq_t)1;
    for (int i = 0; i < cnt; ++i) {
        seq_t per = period_prime_power(f[i], e[i]);
        seq_t g   = gcd(lcm, per);
        lcm = (lcm / g) * per;
    }
    return lcm;
}

/* ------------------------------------------------------------------ */
/* Sequence storage (dynamic)                                         */
/* ------------------------------------------------------------------ */
static seq_t *SEQ      = NULL;
static size_t SEQ_LEN  = 0;
static size_t SEQ_CAP  = 0;
static uint64_t NEXT_K = 10;

/* ------------------------------------------------------------------ */
/* Main entry function                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    build_primes();

    while (SEQ_LEN < (size_t)n) {
        seq_t period = pisano_period(NEXT_K);
        seq_t target = (seq_t)3 * ((seq_t)(NEXT_K + 10));

        if (period == target) {
            if (SEQ_LEN == SEQ_CAP) {
                size_t newcap = SEQ_CAP ? SEQ_CAP * 2 : 16;
                SEQ = (seq_t *)realloc(SEQ, newcap * sizeof(seq_t));
                SEQ_CAP = newcap;
            }
            SEQ[SEQ_LEN++] = (seq_t)NEXT_K;
        }
        NEXT_K += 10;
    }

    return SEQ[n - 1];
}
