/*
 * A162196  Sum of proper divisors minus the number of proper divisors of
 *          nonprime number A018252(n).
 * https://oeis.org/A162196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = σ(A018252(n)) – A018252(n) – τ(A018252(n)) + 1 where
 *            A018252(n) is the n‑th non‑prime integer (starting with 1)
 * Range:     u8 n <= 33, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ----------  integer square root (floor) for uint64_t  ---------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = 0, bit = 1ULL << 62;      /* highest power of 4 <= n */
    while (bit > n) bit >>= 2;
    for (; bit; bit >>= 2) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

/* ----------  prime table and utility -------------------------------- */
static uint64_t *prime_tbl = NULL;      /* dynamic array of primes      */
static size_t prime_cap    = 0;
static size_t prime_cnt    = 0;
static uint64_t sieve_max   = 1;         /* highest number already sieved */

static void extend_primes_up_to(uint64_t limit)
{
    if (limit <= sieve_max) return;

    for (uint64_t n = sieve_max + 1; n <= limit; ++n) {
        if (n < 2) continue;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_tbl[i];
            if (p * p > n) break;
            if (n % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 32;
                uint64_t *tmp = (uint64_t *)realloc(prime_tbl,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_tbl = tmp;
                prime_cap = new_cap;
            }
            prime_tbl[prime_cnt++] = n;
        }
    }
    sieve_max = limit;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    uint64_t sq = isqrt_u64(n);
    extend_primes_up_to(sq);
    for (size_t i = 0; i < prime_cnt && prime_tbl[i] <= sq; ++i) {
        if (n % prime_tbl[i] == 0) return false;
    }
    return true;
}

/* ----------  evaluation of a(n) for nonprime m ---------------------- */
static seq_t eval_a_for(uint64_t m)
{
    seq_t sigma = 1;    /* product of (p^(e+1)-1)/(p-1) terms */
    seq_t tau   = 1;    /* product of (e+1) terms */
    uint64_t tmp = m;

    /* factorisation using known primes up to sqrt(m) */
    for (size_t i = 0; i < prime_cnt && prime_tbl[i] * prime_tbl[i] <= tmp; ++i) {
        uint64_t p = prime_tbl[i];
        if (tmp % p != 0) continue;
        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        /* compute (p^(exp+1)-1)/(p-1) as seq_t */
        seq_t p_pow = 1;
        for (uint64_t k = 0; k < exp; ++k) p_pow *= (seq_t)p;   /* p^exp */
        seq_t term = p_pow * (seq_t)p;          /* p^(exp+1) */
        term -= (seq_t)1;
        term /= (seq_t)(p - 1);
        sigma *= term;
        tau   *= (seq_t)(exp + 1);
    }

    if (tmp > 1) {   /* remaining prime factor */
        sigma *= ((seq_t)tmp + (seq_t)1);
        tau   *= (seq_t)2;
    }

    /* a(m) = sigma(m) – m – tau(m) + 1 */
    return sigma - (seq_t)m - tau + (seq_t)1;
}

/* ----------  sequence cache ---------------------------------------- */
static seq_t *a_cache   = NULL;    /* dynamic array of a(n) values */
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t last_checked = 0;  /* last integer examined for nonprime */

static void extend_cache_to(size_t need)
{
    while (cache_len < need) {
        ++last_checked;
        if (!is_prime_u64(last_checked)) {
            seq_t val = eval_a_for(last_checked);
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(a_cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp) abort();
                a_cache = tmp;
                cache_cap = new_cap;
            }
            a_cache[cache_len++] = val;
        }
    }
}

/* ----------  main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 */
    extend_cache_to((size_t)n);
    return a_cache[(size_t)n - 1];
}
