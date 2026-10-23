/*
 * A163183  Primes dividing 2^j + 1 for some odd j.
 * https://oeis.org/A163183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = the n-th prime p > 2 such that there exists an odd j
 *            with * 2^j ≡ -1 (mod p). The implementation scans primes in
 *            ascending * order, tests each for the condition by computing the
 *            multiplicative * order of 2 modulo p, and caches the found
 *            terms. * * The algorithm uses a simple Sieve of Era...
 * Range:     u8 n <= 15, u16 n <= 1776, u32 n <= 2300, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------
 *  Helper: modular exponentiation for arbitrary exponents
 * -------------------------------------------------------------------- */
static seq_t mod_pow_seq(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    base        %= mod;
    while (exp != 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* --------------------------------------------------------------------
 *  Helper: generate all primes ≤ limit (inclusive) using a simple
 *  sieve.  The caller must free *primes_out.
 * -------------------------------------------------------------------- */
static void simple_sieve(size_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_comp) abort();

    /* Mark composites */
    for (size_t i = 2; i <= limit / i; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_comp[j] = true;
            }
        }
    }

    /* Count primes */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) ++count;
    }

    /* Store primes */
    uint64_t *primes = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!primes) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[idx++] = (uint64_t)i;
        }
    }

    free(is_comp);
    *primes_out = primes;
    *cnt_out    = count;
}

/* --------------------------------------------------------------------
 *  Global state
 * -------------------------------------------------------------------- */
static uint64_t *prime_list   = NULL;   /* primes up to prime_limit */
static size_t    prime_cnt    = 0;      /* number of primes in the list */
static size_t    prime_idx    = 0;      /* next prime to examine */
static size_t    prime_limit  = 0;      /* current sieve limit */

static seq_t   *seq_cache     = NULL;   /* cached a(n) values */
static size_t  seq_cache_len  = 0;      /* number of cached values */
static size_t  seq_cache_cap  = 0;      /* allocated capacity of cache */

/* --------------------------------------------------------------------
 *  Prime list handling
 * -------------------------------------------------------------------- */
static void extend_prime_list(size_t new_limit)
{
    uint64_t *new_primes = NULL;
    size_t   new_cnt    = 0;
    simple_sieve(new_limit, &new_primes, &new_cnt);

    free(prime_list);
    prime_list = new_primes;
    prime_cnt  = new_cnt;
    prime_limit = new_limit;
}

static void init_prime_list(void)
{
    if (prime_list) return;            /* already initialized */
    extend_prime_list(2000000);        /* initial generous limit */
}

/* --------------------------------------------------------------------
 *  Sequence cache handling
 * -------------------------------------------------------------------- */
static void grow_seq_cache(size_t needed)
{
    if (needed <= seq_cache_cap) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!newp) abort();
    seq_cache     = newp;
    seq_cache_cap = new_cap;
}

/* --------------------------------------------------------------------
 *  Test whether prime p satisfies the sequence condition
 * -------------------------------------------------------------------- */
static bool prime_in_seq(uint64_t p)
{
    /* The order of 2 mod p divides p-1.  Start with p-1 and reduce. */
    seq_t ord = (seq_t)p - 1;

    /* Factor ord into distinct prime factors.  We only need a list
     * of the prime factors (not multiplicities). */
    uint64_t factors[32];
    size_t   fcnt   = 0;
    uint64_t tmp    = (uint64_t)ord;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t q = prime_list[i];
        if (tmp % q == 0) {
            factors[fcnt++] = q;
            while (tmp % q == 0) tmp /= q;
        }
    }
    if (tmp > 1 && fcnt < sizeof(factors)/sizeof(factors[0])) {
        factors[fcnt++] = tmp;          /* remaining prime factor */
    }

    /* Reduce ord by each factor while it still yields a residue of 1. */
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t f = factors[i];
        while ((ord % f == 0) &&
               mod_pow_seq((seq_t)2, ord / f, (seq_t)p) == 1) {
            ord /= f;
        }
    }

    /* Count the exponent of 2 in ord (i.e., v2(ord)). */
    size_t v2 = 0;
    seq_t tmp_ord = ord;
    while ((tmp_ord & 1) == 0) {
        tmp_ord >>= 1;
        ++v2;
    }

    return v2 == 1;   /* exactly one factor of 2 */
}

/* --------------------------------------------------------------------
 *  Build the sequence cache up to index n
 * -------------------------------------------------------------------- */
static void ensure_sequence_up_to(idx_t n)
{
    if ((idx_t)seq_cache_len >= n) return;   /* already enough */

    init_prime_list();                         /* ensure prime list ready */

    while ((idx_t)seq_cache_len < n) {
        if (prime_idx >= prime_cnt) {          /* need more primes */
            size_t new_limit = prime_limit * 2;
            extend_prime_list(new_limit);
        }

        uint64_t p = prime_list[prime_idx++];
        if (p < 3) continue;                  /* skip 2 if present */

        if (prime_in_seq(p)) {
            grow_seq_cache(seq_cache_len + 1);
            seq_cache[seq_cache_len++] = (seq_t)p;
        }
    }
}

/* --------------------------------------------------------------------
 *  Public function
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined, safeguard */
    ensure_sequence_up_to(n);
    return seq_cache[n - 1];
}
