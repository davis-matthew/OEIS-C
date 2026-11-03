/*
 * A134189  a(1)=1. Thereafter, a(n) = n*a(n-1) if the number of divisors of
 *          n*a(n-1) is <= the number of divisors of n+a(n-1) or a(n) =
 *          n+a(n-1) if n*a(n-1) has more divisors than n+a(n-1).
 * https://oeis.org/A134189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   gcd, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A134189
 * a(1) = 1
 * For n > 1 let
 *   P = n * a(n-1)
 *   S = n + a(n-1)
 * If d(P) <= d(S) then a(n) = P else a(n) = S
 * where d(x) is the number of positive divisors of x. */
#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

static seq_t *cache = NULL;          /* 0‑based array of already known terms, a(1) at cache[0] */
static size_t cache_len = 0;         /* number of terms currently stored */
static size_t cache_cap = 0;         /* allocated capacity of cache */

/* previous term a(n-1) and its divisor count d(a(n-1)) */
static seq_t a_prev = 1;
static seq_t d_prev = 1;

/* current factorisation of a_prev:
 * fac_primes[i] ^ fac_exps[i]  (i = 0 .. prime_count-1) */
#define MAX_FACTORS 256
static seq_t fac_primes[MAX_FACTORS];
static unsigned char fac_exps[MAX_FACTORS];
static size_t prime_count = 0;

/* small factorisation for n <= 10000 */
static void factor_small(idx_t n,
                         seq_t *pr, unsigned char *ex, size_t *cnt)
{
    idx_t t = n;
    size_t c = 0;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            ex[c] = 0;
            pr[c] = (seq_t)p;
            while (t % p == 0) {
                t /= p;
                ex[c]++;
            }
            ++c;
        }
    }
    if (t > 1) {
        pr[c] = (seq_t)t;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Pollard Rho and related primitives for large seq_t values */
static seq_t pollards_rho(seq_t n)
{
    if ((n & (seq_t)1) == (seq_t)0) return (seq_t)2;
    seq_t c = (seq_t)1;
    seq_t x = (seq_t)2;
    seq_t y = (seq_t)2;
    seq_t d = (seq_t)1;
    while (d == (seq_t)1) {
        x = ((x * x) + c);
        x %= n;
        y = ((y * y) + c);
        y %= n;
        y = ((y * y) + c);
        y %= n;
        seq_t diff = (x > y) ? (x - y) : (y - x);
        d = gcd(diff, n);
        if (d == n) {
            c++;                     /* try another constant    */
            x = (seq_t)2;
            y = (seq_t)2;
            d = (seq_t)1;
        }
    }
    return d;
}

static void factor_rho_rec(seq_t n,
                           seq_t *buf, size_t *len)
{
    if (n <= (seq_t)1) return;
    if (is_prime_big(n)) {
        buf[(*len)++] = n;
        return;
    }
    seq_t d = pollards_rho(n);
    factor_rho_rec(d, buf, len);
    factor_rho_rec(n / d, buf, len);
}

/* compute number of divisors of x (uses Pollard Rho) */
static seq_t count_divisors(seq_t x)
{
    if (x <= (seq_t)1) return (seq_t)1;
    seq_t buf[1024];
    size_t flen = 0;
    factor_rho_rec(x, buf, &flen);
    mem_qsort(buf, flen, sizeof(seq_t), cmp_seq_t);
    seq_t total = (seq_t)1;
    for (size_t i = 0; i < flen;) {
        size_t j = i;
        while (j < flen && buf[j] == buf[i]) ++j;
        total *= (seq_t)(j - i + 1);
        i = j;
    }
    return total;
}

/* find the index of a prime p in the current factorisation, or -1 */
static int find_prime(seq_t p)
{
    for (size_t i = 0; i < prime_count; ++i) {
        if (fac_primes[i] == p) return (int)i;
    }
    return -1;
}

/* initialise or grow the cache up to index n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;          /* already computed up to n */

    /* initialise first term if needed */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = mem_resize(seq_t, cache, cache_cap);
        cache[0] = a_prev;
        cache_len = 1;
        a_prev = (seq_t)1;
        d_prev = (seq_t)1;
        prime_count = 0;
    }

    for (size_t idx = cache_len + 1; idx <= n; ++idx) {
        idx_t cur = (idx_t)idx;
        seq_t cur_seq = (seq_t)cur;

        /* factorise cur (small) */
        seq_t factor_pr[20];
        unsigned char factor_exp[20];
        size_t factor_cnt = 0;
        factor_small(cur, factor_pr, factor_exp, &factor_cnt);

        /* compute divisor count of product P without changing state */
        seq_t dprod = d_prev;
        for (size_t k = 0; k < factor_cnt; ++k) {
            seq_t p = factor_pr[k];
            unsigned char exp_n = factor_exp[k];
            int idxp = find_prime(p);
            unsigned char oldExp = (idxp >= 0) ? fac_exps[idxp] : 0;
            seq_t newExp = (seq_t)oldExp + exp_n;
            dprod = dprod / ((seq_t)oldExp + (seq_t)1);
            dprod = dprod * (newExp + (seq_t)1);
        }

        /* compute sum S and its divisor count */
        seq_t sum_val = a_prev + cur_seq;

        /* factorise sum */
        seq_t sum_buf[1024];
        size_t sum_flen = 0;
        factor_rho_rec(sum_val, sum_buf, &sum_flen);
        mem_qsort(sum_buf, sum_flen, sizeof(seq_t), cmp_seq_t);

        seq_t sum_primes[1024];
        unsigned char sum_exps[1024];
        size_t sum_cnt = 0;
        for (size_t i = 0; i < sum_flen;) {
            size_t j = i;
            while (j < sum_flen && sum_buf[j] == sum_buf[i]) ++j;
            sum_primes[sum_cnt] = sum_buf[i];
            sum_exps[sum_cnt] = (unsigned char)(j - i);
            ++sum_cnt;
            i = j;
        }

        seq_t dsum = (seq_t)1;
        for (size_t i = 0; i < sum_cnt; ++i) dsum *= (sum_exps[i] + (seq_t)1);

        /* choose the next term */
        if (dprod <= dsum) {                /* use product */
            a_prev = a_prev * cur_seq;

            /* update factorisation to reflect product */
            for (size_t k = 0; k < factor_cnt; ++k) {
                seq_t p = factor_pr[k];
                unsigned char exp_n = factor_exp[k];
                int idxp = find_prime(p);
                if (idxp >= 0) {
                    fac_exps[idxp] += exp_n;
                } else {
                    fac_primes[prime_count] = p;
                    fac_exps[prime_count] = exp_n;
                    ++prime_count;
                }
            }
            d_prev = dprod;
        } else {                              /* use sum */
            a_prev = sum_val;

            /* replace factorisation with that of the sum */
            for (size_t i = 0; i < sum_cnt; ++i) {
                fac_primes[i] = sum_primes[i];
                fac_exps[i] = sum_exps[i];
            }
            prime_count = sum_cnt;
            d_prev = dsum;
        }

        /* store computed term */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_len] = a_prev;
        ++cache_len;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
