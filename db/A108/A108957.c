/*
 * A108957  Values of n such that n - 2^k is deficient for all 1 <= 2^k < n.
 * https://oeis.org/A108957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A108957: n such that n – 2^k is deficient for every 1 ≤ 2^k < n.
    The function builds the sequence on demand, memoising the values.
    It precomputes primes up to 200 000 (more than enough for the
    first 10 000 terms), calculates σ(m) multiplicatively and checks
    σ(m) < 2·m to determine deficiency.  All arithmetic fits into a
    128‑bit unsigned integer; any arbitrary‑precision build will
    handle the same logic correctly. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* ---------- prime table ---------- */
static uint32_t *primes = NULL;
static size_t primes_size = 0;

static void init_primes(size_t limit)
{
    if (primes != NULL) return;          /* already initialised */

    size_t n = limit + 1;
    uint8_t *is_prime = (uint8_t*)calloc(n, 1);
    assert(is_prime);
    for (size_t i = 2; i < n; ++i) is_prime[i] = 1;

    for (size_t p = 2; p * p < n; ++p) if (is_prime[p]) {
        for (size_t j = p * p; j < n; j += p) is_prime[j] = 0;
    }

    primes_size = 0;
    for (size_t i = 2; i < n; ++i) if (is_prime[i]) primes_size++;

    primes = (uint32_t*)malloc(primes_size * sizeof(uint32_t));
    assert(primes);
    size_t idx = 0;
    for (size_t i = 2; i < n; ++i) if (is_prime[i]) primes[idx++] = (uint32_t)i;

    free(is_prime);
}

/* ---------- divisor sum ---------- */
static unsigned __int128 sigma_of(uint64_t m)
{
    if (m == 1) return 1;
    unsigned __int128 prod = 1;
    uint64_t temp = m;

    for (size_t i = 0; i < primes_size && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            unsigned __int128 term = 1;
            unsigned __int128 cur = 1;
            while (temp % p == 0) {
                temp /= p;
                cur *= p;
                term += cur;
            }
            prod *= term;
        }
    }
    if (temp > 1) {
        prod *= (unsigned __int128)temp + 1;    /* remaining prime factor */
    }
    return prod;
}

static bool is_deficient(uint64_t m)
{
    unsigned __int128 sigma = sigma_of(m);
    return sigma < (unsigned __int128)2 * m;
}

/* ---------- memoisation ---------- */
static seq_t *terms = NULL;
static size_t terms_size = 0;
static size_t terms_cap = 0;
static uint64_t next_candidate = 2;

static void add_term(seq_t v)
{
    if (terms_size >= terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 8 : terms_cap * 2;
        seq_t *new_arr = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        assert(new_arr);
        terms = new_arr;
        terms_cap = new_cap;
    }
    terms[terms_size++] = v;
}

/* ---------- public routine ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* off‑by‑one is irrelevant for offset 1 */

    /* initialise primes once */
    static bool primes_inited = false;
    if (!primes_inited) {
        init_primes(200000);   /* ample for first 10 000 terms */
        primes_inited = true;
    }

    /* generate terms until we have at least n */
    while (terms_size < (size_t)n) {
        uint64_t cand = next_candidate++;
        bool ok = true;
        for (uint64_t pow = 1; pow < cand; pow <<= 1) {
            uint64_t m = cand - pow;
            if (!is_deficient(m)) {
                ok = false;
                break;
            }
        }
        if (ok) add_term((seq_t)cand);
    }
    return terms[n-1];
}
