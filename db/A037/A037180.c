/*
 * A037180  Number of different cycle lengths when squaring modulo n-th prime.
 * https://oeis.org/A037180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct multiplicative orders of 2 modulo d
 *            for all odd divisors d of (p-1), where p is the n-th prime
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   divisor_list
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/divisor_list.h"

/* --------------------------------------------------------------------- */
/*  Prime generation (sieve up to 200000 – sufficient for n ≤ 10000)    */
static size_t prime_count = 0;
static idx_t *prime_list = NULL;

static void init_primes(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;
    const idx_t LIMIT = 200000;
    uint8_t *is_comp = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
    if (!is_comp) exit(1);
    memset(is_comp, 0, LIMIT + 1);
    prime_list = (idx_t *)malloc((LIMIT + 1) * sizeof(idx_t));
    if (!prime_list) { free(is_comp); exit(1); }

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_list[prime_count++] = i;
            if (i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i) is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
}

static inline idx_t nth_prime(idx_t k)
{
    init_primes();
    if (k == 0 || (idx_t)k > prime_count) return 0;
    return prime_list[k - 1];
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/*  Helper: modular exponentiation (uint64_t)                            */
static seq_t powmod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t b   = base % mod;
    while (exp) {
        if (exp & 1) res = (res * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return res;
}

/* --------------------------------------------------------------------- */
/*  Helper: multiplicative order of 2 modulo d (d odd, >0)              */
static seq_t multiplicative_order_mod2(idx_t d)
{
    if (d == 1) return 1;

    /* Factor d to compute φ(d) */
    idx_t primes[16], exps[16];
    int cnt = 0;
    idx_t tmp = d;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[cnt] = p; exps[cnt] = e; ++cnt;
        }
    }
    if (tmp > 1) { primes[cnt] = tmp; exps[cnt] = 1; ++cnt; }

    idx_t phi = d;
    for (int i = 0; i < cnt; ++i)
        phi = phi / primes[i] * (primes[i] - 1);

    /* Factor φ(d) to reduce the exponent */
    idx_t pf[16], pe[16];
    int pf_cnt = 0;
    idx_t tmp2 = phi;
    for (idx_t p = 2; p * p <= tmp2; ++p) {
        if (tmp2 % p == 0) {
            int e = 0;
            while (tmp2 % p == 0) { tmp2 /= p; ++e; }
            pf[pf_cnt] = p; pe[pf_cnt] = e; ++pf_cnt;
        }
    }
    if (tmp2 > 1) { pf[pf_cnt] = tmp2; pe[pf_cnt] = 1; ++pf_cnt; }

    seq_t ord = phi;
    for (int i = 0; i < pf_cnt; ++i) {
        idx_t p = pf[i];
        while (ord % p == 0) {
            if (powmod_u64(2ULL, (uint64_t)(ord / p), (uint64_t)d) == 1)
                ord /= p;
            else
                break;
        }
    }
    return ord;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
seq_t a(idx_t n)
{
    idx_t p = nth_prime(n);
    if (p == 0) return 0;          /* invalid input */

    idx_t m = p - 1;
    while ((m & 1) == 0) m >>= 1;  /* remove all powers of two */
    idx_t t = m;                    /* odd part of p-1 */

    /* get all divisors of t (t <= 100000) */
    idx_t divisors[256];
    size_t dcnt;
    divisor_list(t, divisors, &dcnt);

    seq_t orders[256];
    size_t ocnt = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        seq_t ord = multiplicative_order_mod2(divisors[i]);

        bool seen = false;
        for (size_t j = 0; j < ocnt; ++j) {
            if (orders[j] == ord) { seen = true; break; }
        }
        if (!seen) orders[ocnt++] = ord;
    }
    return (seq_t)ocnt;
}
