/*
 * A073025  Primes p such that p-1 has a prime factor q > p^(3/4).
 * https://oeis.org/A073025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   compute OEIS sequence A073025 * a(n) = the n-th prime p such
 *            that p-1 has a prime divisor q > p^(3/4)
 * Range:     u8 none, u16 n <= 1, u32 n <= 28, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   build_primes_upto, build_spf, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prerequisites from utils */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"
#include "../utils/seq_pow.h"

/* forward declarations for utility functions used */
extern void free_cache(seq_t *p);  /* unused but harmless */

static const idx_t MAX_LIMIT = 2000000;   /* enough for n <= 10000 */
static uint64_t *primes = NULL;          /* array of primes up to MAX_LIMIT */
static size_t primes_cnt = 0;
static uint64_t *spf = NULL;             /* smallest prime factor array */
static idx_t *values = NULL;            /* memoized sequence values */
static size_t values_len = 0;
static size_t values_cap = 0;
static idx_t prime_idx = 0;              /* next prime index to test */

static void init(void)
{
    /* build primes up to MAX_LIMIT */
    build_primes_upto(MAX_LIMIT, &primes, &primes_cnt);

    /* build SPF array up to MAX_LIMIT */
    build_spf(MAX_LIMIT, &spf);
}

static uint64_t largest_prime_factor(uint64_t n)
{
    uint64_t max_pf = 0;
    while (n > 1) {
        uint64_t f = spf[n];
        if (f > max_pf) max_pf = f;
        n /= f;
    }
    return max_pf;
}

static void add_value(seq_t val)
{
    if (values_len >= values_cap) {
        size_t new_cap = values_cap == 0 ? 64 : values_cap * 2;
        idx_t *tmp = (idx_t*)realloc(values, new_cap * sizeof(idx_t));
        values = tmp;
        values_cap = new_cap;
    }
    values[values_len++] = (idx_t)val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, return 0 */

    if (values_len == 0) init();

    while (values_len < n) {
        if (prime_idx >= primes_cnt) return 0;  /* safety, should not happen */

        uint64_t p = primes[prime_idx++];
        /* skip very small primes that cannot satisfy */
        if (p < 23) continue;

        uint64_t pm1 = p - 1;
        uint64_t q = largest_prime_factor(pm1);
        if (q <= 1) continue;

        seq_t p_seq = (seq_t)p;
        seq_t q_seq = (seq_t)q;

        /* compare q^4 > p^3 */
        seq_t q4 = seq_pow(q_seq, 4);
        seq_t p3 = seq_pow(p_seq, 3);

        if (q4 > p3) {
            add_value(p_seq);
        }
    }

    return values[n - 1];
}
