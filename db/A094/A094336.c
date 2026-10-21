/*
 * A094336  Product of next n numbers divided by the largest factorial
 *          divisor.
 * https://oeis.org/A094336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 11, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A094336: a(n) = [product_{k=1}^n (n(n-1)/2 + k)] / [largest factorial divisor] */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* power function used for arbitrary precision arithmetic */
#include "../utils/seq_pow.h"

/* ----- prime sieve for numbers up to sqrt(5e7) ----- */
static uint32_t primes[900];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0) return;
    /* 5e7 < 2^31, so sqrt < 7072 */
    const int LIM = 7072;
    bool mark[LIM + 1];
    memset(mark, 1, sizeof(mark));
    mark[0] = mark[1] = 0;
    for (int i = 2; i * i <= LIM; ++i)
        if (mark[i]) {
            for (int j = i * i; j <= LIM; j += i)
                mark[j] = 0;
        }
    for (int i = 2; i <= LIM; ++i)
        if (mark[i]) primes[prime_cnt++] = (uint32_t)i;
}

/* ----- simple hash table for prime exponents ----- */
#define TABLE_SIZE (1u << 16)  /* power of two, > 2 * expected number of distinct primes */

typedef struct {
    uint32_t key;   /* prime */
    idx_t val;      /* exponent */
    unsigned used;  /* 1 if slot occupied */
} Entry;

static Entry  hash_table[TABLE_SIZE];
static size_t used_idx[TABLE_SIZE];   /* indices of slots that were used */
static size_t used_cnt = 0;

/* hash function: simple mask */
static inline size_t hash_key(uint32_t k)
{
    return k & (TABLE_SIZE - 1);
}

/* add delta (can be negative) to exponent of prime key */
static void inc_prime(uint32_t key, idx_t delta)
{
    size_t h = hash_key(key);
    while (hash_table[h].used && hash_table[h].key != key)
        h = (h + 1) & (TABLE_SIZE - 1);
    if (!hash_table[h].used) {         /* new entry */
        hash_table[h].used = 1;
        hash_table[h].key  = key;
        hash_table[h].val  = delta;
        used_idx[used_cnt++] = h;
    } else
        hash_table[h].val += delta;
}

/* read exponent of prime key, 0 if absent */
static inline idx_t get_prime(uint32_t key)
{
    size_t h = hash_key(key);
    while (hash_table[h].used && hash_table[h].key != key)
        h = (h + 1) & (TABLE_SIZE - 1);
    if (!hash_table[h].used) return 0;
    return hash_table[h].val;
}

/* clear hash table for next call */
static void clear_hash(void)
{
    for (size_t i = 0; i < used_cnt; ++i) {
        hash_table[used_idx[i]].used = 0;
        hash_table[used_idx[i]].val  = 0;
    }
    used_cnt = 0;
}

/* factor a number x (<= 5e7) and add/subtract its exponent count */
static void factor_and_inc(uint64_t x, idx_t delta)
{
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = primes[pi];
        uint64_t pp = (uint64_t)p * p;
        if (pp > x) break;
        if (x % p == 0) {
            idx_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            inc_prime(p, cnt * delta);
        }
    }
    if (x > 1) {
        /* remaining factor is a prime > sqrt(5e7) */
        inc_prime((uint32_t)x, delta);
    }
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n < 1 */

    static bool primes_inited = false;
    if (!primes_inited) { init_primes(); primes_inited = true; }

    clear_hash();

    /* compute the range [A, B] of consecutive integers */
    const uint64_t A = ((uint64_t)n * (n - 1)) / 2 + 1;
    const uint64_t B = A + n - 1;

    /* factor each number in the range and accumulate exponents */
    for (uint64_t v = A; v <= B; ++v)
        factor_and_inc(v, +1);

    /* determine the largest m such that m! divides the product */
    idx_t m = 1;
    for (;;) {
        idx_t t = m + 1;               /* candidate factorial */
        uint64_t y = t;

        /* factor t and store primes + exponents in a small buffer */
        uint32_t fac[16];
        idx_t   cnt[16];
        size_t  sz = 0;

        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            uint32_t p = primes[pi];
            uint64_t pp = (uint64_t)p * p;
            if (pp > y) break;
            if (y % p == 0) {
                cnt[sz] = 0;
                do { y /= p; ++cnt[sz]; } while (y % p == 0);
                fac[sz++] = p;
            }
        }
        if (y > 1) { fac[sz] = (uint32_t)y; cnt[sz] = 1; ++sz; }

        /* check if the product still contains all required primes */
        bool ok = true;
        for (size_t i = 0; i < sz; ++i)
            if (get_prime(fac[i]) < cnt[i]) { ok = false; break; }

        if (!ok) break;                /* cannot increase m any more */

        /* subtract the exponents used by this factorial factor */
        for (size_t i = 0; i < sz; ++i)
            inc_prime(fac[i], -cnt[i]);

        m = t;
    }

    /* compute the remaining product: product / m! */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < used_cnt; ++i) {
        size_t idx = used_idx[i];
        idx_t   exp = hash_table[idx].val;
        if (exp > 0)
            result = result * seq_pow((seq_t)hash_table[idx].key, exp);
    }

    return result;
}
