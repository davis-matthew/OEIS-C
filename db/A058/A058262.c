/*
 * A058262  a(n) is the quotient obtained when the totient of primorial (that
 *          is, the product of p-1 values) is divided by the LCM of the same
 *          p-1 values.
 * https://oeis.org/A058262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(∏_{i=0..n} p_i) / lcm_{i=0..n} (p_i-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 12, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, build_spf, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here p_i is the i‑th prime (p_0 = 2).  Equivalently, */
/* a(n) = ∏_q q^{(∑_i v_q(p_i-1) – max_i v_q(p_i-1))}, where q ranges over all primes. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"  /* build_primes_upto(idx_t limit, uint64_t **primes, size_t *cnt) */
#include "../utils/build_spf.h"          /* build_spf(idx_t limit, idx_t **spf_out) */
#include "../utils/big_pow.h"            /* seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/seq_mul.h"            /* seq_t seq_mul(seq_t a, seq_t b) */

#define PRIME_LIMIT  300000   /* primes up to 300 000 give ~25 000 primes, more than enough for n≈10 000 */
#define SPF_LIMIT (PRIME_LIMIT)

static uint64_t *prime_list  = NULL;  /* array of primes, prime_list[0] = 2 */
static size_t   prime_cnt    = 0;

static idx_t *spf_array = NULL;      /* smallest prime factor for numbers < SPF_LIMIT+1 */

static seq_t *cache = NULL;          /* memoised a(n) values */
static size_t cache_len = 0;         /* number of values actually stored */
static size_t cache_cap = 0;         /* allocated slots in cache */

static unsigned int *sum_exp = NULL;   /* sum of exponents of prime q in all (p_i-1) processed so far */
static unsigned int *max_exp = NULL;   /* maximum exponent of prime q among those numbers */

/* ------------------------------------------------------------------ */
/* lazy global initialisation                                          */
static void init_global(void)
{
    if (cache) return;   /* already initialised */

    /* Allocate cache for a few terms initially. */
    cache_cap = 128;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();           /* fatal if allocation fails */
    cache[0] = (seq_t)1;           /* a(0) = 1 */
    cache_len = 1;

    /* Build primes up to PRIME_LIMIT. */
    build_primes_upto(PRIME_LIMIT, &prime_list, &prime_cnt);

    /* Build smallest prime factor array. */
    build_spf(SPF_LIMIT, &spf_array);

    /* Allocate sum and max exponent tables. */
    sum_exp = (unsigned int*)calloc(SPF_LIMIT + 1, sizeof(unsigned int));
    max_exp = (unsigned int*)calloc(SPF_LIMIT + 1, sizeof(unsigned int));
}

/* ------------------------------------------------------------------ */
static void grow_cache(size_t needed)
{
    if (needed < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_global();

    if (n + 1 > cache_len) {
        grow_cache((size_t)n + 1);
        for (size_t cur = cache_len; cur <= (size_t)n; ++cur) {
            /* previous value */
            seq_t val = cache[cur - 1];

            /* prime p_cur */
            uint64_t p = prime_list[cur];
            uint64_t m = p - 1;      /* number to factorise */

            uint64_t x = m;
            while (x > 1) {
                uint32_t q = (uint32_t)spf_array[x];
                uint32_t e = 0;
                while (x % q == 0) { x /= q; ++e; }

                unsigned int max_old = max_exp[q];
                /* update multiplicative factor */
                if (e <= max_old) {
                    seq_t mult = big_pow((seq_t)q, (seq_t)e);
                    val = seq_mul(val, mult);
                } else {
                    seq_t mult = big_pow((seq_t)q, (seq_t)max_old);
                    val = seq_mul(val, mult);
                }

                /* update exponent tables */
                sum_exp[q] += e;
                if (e > max_exp[q]) max_exp[q] = e;
            }

            cache[cur] = val;
            ++cache_len;
        }
    }
    return cache[n];
}
