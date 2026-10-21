/*
 * A079581  Consider pairs (r,s) such that the polynomial (x^r+1) divides
 *          (x^s+1) and 1 <= r < s. This sequence gives the s values; A079673
 *          gives the r values.
 * https://oeis.org/A079581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth element of the multiset containing each integer
 *            s≥1 repeated (d_odd(s)-1) times, where d_odd(s) is the number of
 *            odd divisors of s. (x^r+1) divides (x^s+1) iff s/r is odd
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- static state ---------- */
static uint8_t  *prime_sieve = NULL;   /* intermediate sieve buffer */
static idx_t     sieve_limit = 0;      /* highest number sieved */
static uint32_t *primes = NULL;        /* list of primes */
static size_t    primes_cnt = 0;       /* count of primes */
static size_t    primes_cap = 0;       /* capacity of primes array */

static seq_t   *term_array = NULL;     /* sequence values */
static size_t    term_len   = 0;        /* number of terms stored */
static size_t    term_cap   = 0;        /* capacity of term_array */
static idx_t     next_s     = 1;        /* next s to examine */

/* ---------- prime generation ---------- */
static void generate_primes_upto(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* simple sieve of Eratosthenes */
    uint8_t *is_prime = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    if (!is_prime) exit(EXIT_FAILURE);
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p) is_prime[j] = 0;
        }
    }
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 64;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }
    free(is_prime);
    sieve_limit = limit;
}

/* ---------- odd divisor count ---------- */
static size_t odd_divisor_count(idx_t n)
{
    /* remove powers of two */
    while ((n & 1) == 0) n >>= 1;

    size_t count = 1;
    idx_t temp = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
    }
    if (temp > 1) count *= 2;          /* remaining prime factor */
    return count;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* off‑by‑one protection */

    /* lazy initialization */
    if (!term_array) {
        term_array = NULL; term_len = 0; term_cap = 0;
        next_s = 1;
        /* enough primes for s up to ~200000 (covers n≈10000) */
        generate_primes_upto(200000);
    }

    while (term_len < n) {
        ++next_s;
        size_t occ = odd_divisor_count(next_s);
        if (occ <= 1) continue;                /* no r < s */
        occ -= 1;                               /* exclude r = s */

        size_t need = term_len + occ;
        if (term_cap < need) {
            size_t new_cap = term_cap ? term_cap * 2 : 64;
            while (new_cap < need) new_cap *= 2;
            term_array = (seq_t*)realloc(term_array, new_cap * sizeof(seq_t));
            if (!term_array) exit(EXIT_FAILURE);
            term_cap = new_cap;
        }

        for (size_t i = 0; i < occ; ++i)
            term_array[term_len++] = (seq_t)next_s;
    }

    return term_array[n - 1];
}
