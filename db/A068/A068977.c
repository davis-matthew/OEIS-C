/*
 * A068977  Numbers k such that Sum_{d|k} d/tau(d) is an integer, where tau(x)
 *          = A000005(x).
 * https://oeis.org/A068977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 7738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th integer k such that
   Sum_{d|k} d / tau(d) is an integer.  The function
   incrementally finds the terms by testing successive k. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime sieve – sufficient for k values up to about 10⁶.              */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 1000000U

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_built = false;

static void build_primes(void)
{
    if (primes_built) return;
    bool *is_prime = (bool*)calloc(PRIME_LIMIT + 1U, sizeof(bool));
    if (!is_prime) exit(1);
    for (uint32_t i = 2U; i <= PRIME_LIMIT; ++i) is_prime[i] = true;

    for (uint32_t i = 2U; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (!is_prime[i]) continue;
        for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i) is_prime[j] = false;
    }

    size_t cnt = 0;
    for (uint32_t i = 2U; i <= PRIME_LIMIT; ++i) if (is_prime[i]) ++cnt;
    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!primes) exit(1);
    size_t idx = 0;
    for (uint32_t i = 2U; i <= PRIME_LIMIT; ++i) if (is_prime[i]) primes[idx++] = i;
    primes_cnt = cnt;
    free(is_prime);
    primes_built = true;
}

/* ------------------------------------------------------------------ */
/* Euclidean gcd for unsigned 128‑bit values.                          */
/* ------------------------------------------------------------------ */
static unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Compute S(p^e) as a reduced fraction (num/den).                    */
/* ------------------------------------------------------------------ */
static void prime_power_sum(uint32_t p, unsigned exp,
                            unsigned __int128 *num_out,
                            unsigned __int128 *den_out)
{
    unsigned __int128 num = 0;
    unsigned __int128 den = 1;
    unsigned __int128 pow = 1;          /* p^i */
    for (unsigned i = 0; i <= exp; ++i) {
        unsigned __int128 term_num = pow;
        unsigned __int128 term_den = (unsigned __int128)(i + 1);

        unsigned __int128 new_num = num * term_den + den * term_num;
        unsigned __int128 new_den = den * term_den;
        unsigned __int128 g = gcd128(new_num, new_den);

        num = new_num / g;
        den = new_den / g;

        pow *= (unsigned __int128)p;
    }
    *num_out = num;
    *den_out = den;
}

/* ------------------------------------------------------------------ */
/* Test whether S(k) is an integer by multiplicativity.               */
/* ------------------------------------------------------------------ */
static bool good_k(uint64_t k)
{
    unsigned __int128 prod_num = 1;
    unsigned __int128 prod_den = 1;
    uint64_t tmp = k;
    for (size_t idx = 0; idx < primes_cnt &&
                         (uint64_t)primes[idx] * (uint64_t)primes[idx] <= tmp; ++idx) {
        uint32_t p = primes[idx];
        if (tmp % p != 0U) continue;
        unsigned exp = 0U;
        while (tmp % p == 0U) { tmp /= p; ++exp; }

        unsigned __int128 np, dp;
        prime_power_sum(p, exp, &np, &dp);

        unsigned __int128 new_num = prod_num * np;
        unsigned __int128 new_den = prod_den * dp;
        unsigned __int128 g = gcd128(new_num, new_den);

        prod_num = new_num / g;
        prod_den = new_den / g;
    }
    if (tmp > 1U) {
        unsigned __int128 np, dp;
        prime_power_sum((uint32_t)tmp, 1U, &np, &dp);

        unsigned __int128 new_num = prod_num * np;
        unsigned __int128 new_den = prod_den * dp;
        unsigned __int128 g = gcd128(new_num, new_den);

        prod_num = new_num / g;
        prod_den = new_den / g;
    }
    return prod_den == 1U;
}

/* ------------------------------------------------------------------ */
/* Dynamic array for storing the terms of the sequence.                */
/* ------------------------------------------------------------------ */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;

static void push_term(seq_t v)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2U : 64U;
        seq_t *new_terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_terms) exit(1);
        terms = new_terms;
        terms_cap = new_cap;
    }
    terms[terms_len++] = v;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0U;   /* undefined but harmless */

    build_primes();

    static uint64_t last_checked = 0U;
    while (terms_len < (size_t)n) {
        ++last_checked;
        if (good_k(last_checked))
            push_term((seq_t)last_checked);
    }
    return terms[(size_t)n - 1U];
}
