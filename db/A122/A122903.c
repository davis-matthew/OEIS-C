/*
 * A122903  Numbers m such that in Z/mZ the number of squares is equal to the
 *          number of invertible elements.
 * https://oeis.org/A122903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes members of OEIS sequence A122903.
   A number m is in the sequence if the number of distinct quadratic residues
   modulo m equals φ(m) (Euler's totient function).  The implementation
   iteratively generates candidates m, checks the condition, caches the
   first n terms, and returns the nth term.  The sequence is generated
   independently of the index n, so calls may come in arbitrary order.
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint32_t *primes      = NULL; /* list of odd primes */
static size_t  primes_cnt   = 0;
static uint32_t primes_limit = 0;     /* current maximum prime generated */

/* generate all odd primes <= limit using a simple sieve */
static void generate_primes(uint32_t limit)
{
    if (limit <= primes_limit) return;
    uint32_t size = limit + 1;
    bool *is_comp = (bool *)malloc(size);
    if (!is_comp) exit(1);
    for (uint32_t i = 0; i <= limit; ++i) is_comp[i] = false;
    for (uint32_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;

    if (primes) free(primes);
    primes_cnt   = 0;
    primes_limit = limit;

    /* rough estimate of number of primes: limit / log(limit) */
    size_t est = limit / 20 + 100;
    primes = (uint32_t *)malloc(est * sizeof(uint32_t));
    if (!primes) exit(1);
    for (uint32_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) {
            if (primes_cnt == est) {
                est *= 2;
                primes = (uint32_t *)realloc(primes, est * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[primes_cnt++] = i;
        }
    free(is_comp);
}

/* check whether m satisfies the condition */
static bool is_valid(uint64_t m)
{
    if (m == 1) return true;

    uint64_t temp = m;
    unsigned a = 0;
    while ((temp & 1ULL) == 0) {
        ++a;
        temp >>= 1;
    }

    /* phi and number of squares for the 2^a part */
    seq_t phi = 1;
    seq_t sq  = 1;

    if (a) {
        if (a == 1)
            phi *= 1ULL;
        else
            phi *= ((seq_t)1 << (a - 1));

        if (a == 1 || a == 2)
            sq = 2;
        else if (a == 3)
            sq = 3;
        else
            sq = ((seq_t)1 << (a - 2));
    }

    /* factorize remaining odd part */
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint32_t p = primes[idx];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0) {
            uint64_t pe = 1;
            while (temp % p == 0) {
                temp /= p;
                pe *= p;
            }
            seq_t pe_t   = (seq_t)pe;
            seq_t phi_p  = pe_t - pe_t / p;
            seq_t sq_p   = phi_p / 2 + 1;
            phi *= phi_p;
            sq  *= sq_p;
        }
    }

    if (temp > 1) { /* prime factor larger than sqrt(m) */
        seq_t pe_t   = (seq_t)temp;
        seq_t phi_p  = pe_t - 1;
        seq_t sq_p   = phi_p / 2 + 1;
        phi *= phi_p;
        sq  *= sq_p;
    }

    return sq == phi;
}

/* cache for computed sequence values */
struct cache_state {
    seq_t  *data;
    size_t  size;
    size_t  cap;
    uint64_t last_checked;
};
static struct cache_state cache = {NULL, 0, 0, 0};

/* append a new term to the cache */
static void append_to_cache(seq_t val)
{
    if (cache.size == cache.cap) {
        size_t new_cap = cache.cap ? cache.cap * 2 : 128;
        seq_t *new_data = (seq_t *)realloc(cache.data, new_cap * sizeof(seq_t));
        if (!new_data) exit(1);
        cache.data = new_data;
        cache.cap  = new_cap;
    }
    cache.data[cache.size++] = val;
}

seq_t a(idx_t n)
{
    /* initialize primes only once */
    if (!primes_limit) generate_primes(2000000);

    /* generate sequence terms until we have at least n of them */
    while (cache.size < (size_t)n) {
        ++cache.last_checked;
        if (is_valid(cache.last_checked))
            append_to_cache((seq_t)cache.last_checked);
    }
    return cache.data[n - 1];
}
