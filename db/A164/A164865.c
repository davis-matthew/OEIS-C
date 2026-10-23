/*
 * A164865  Sum of the distinct semiprime divisors of the n-th number with two
 *          or more distinct semiprime divisors.
 * https://oeis.org/A164865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of distinct semiprime divisors of the n‑th number
 *            with at least two distinct semiprime divisors. The sequence is
 *            produced by scanning the integers in increasing order and
 *            collecting the sums of their distinct semiprime divisors. The
 *            function is memoised so that calls in any order are e...
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- configuration ---------- */
#define MAX_SPF   2000000       /* sieve limit for smallest‑prime‑factor table */
#define MAX_TERMS 200000        /* maximum number of sequence values kept */

/* ---------- static data ---------- */
static unsigned int *spf = NULL;      /* smallest prime factor table */
static unsigned int *primes = NULL;   /* list of primes */
static size_t prime_len = 0, prime_cap = 0;
static bool spf_ready = false;

static seq_t seq_vals[MAX_TERMS];
static size_t seq_len = 0;
static uint64_t current = 1;          /* next integer to test */

/* ---------- prime sieve ---------- */
static void grow_primes(void)
{
    if (prime_len >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        primes = (unsigned int *)realloc(primes, new_cap * sizeof(unsigned int));
        prime_cap = new_cap;
    }
}

static void init_spf(void)
{
    if (spf_ready) return;

    size_t limit = MAX_SPF;
    spf = (unsigned int *)calloc(limit + 1, sizeof(unsigned int));
    if (!spf) exit(1);

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (unsigned int)i;
            grow_primes();
            primes[prime_len++] = (unsigned int)i;
        }
        for (size_t j = 0; j < prime_len; ++j) {
            unsigned int p = primes[j];
            if (p > spf[i] || i * (size_t)p > limit) break;
            spf[i * (size_t)p] = p;
        }
    }
    spf_ready = true;
}

/* ---------- compute sum of distinct semiprime divisors ---------- */
static inline seq_t compute_semiprime_sum(uint64_t x)
{
    unsigned int local_primes[20];
    unsigned int local_exp[20];
    size_t pcnt = 0;

    uint64_t tmp = x;
    while (tmp > 1) {
        unsigned int p = spf[(size_t)tmp];
        unsigned int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        local_primes[pcnt] = p;
        local_exp[pcnt]   = e;
        ++pcnt;
    }

    /* count distinct semiprime divisors */
    size_t semcnt = 0;
    for (size_t i = 0; i < pcnt; ++i)
        if (local_exp[i] >= 2) ++semcnt;          /* p^2 */
    semcnt += pcnt * (pcnt - 1) / 2;              /* p_i * p_j */

    if (semcnt < 2) return 0;    /* not in the sequence */

    /* sum of distinct semiprime divisors */
    seq_t sum = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        if (local_exp[i] >= 2)
            sum += (seq_t)local_primes[i] * (seq_t)local_primes[i]; /* p^2 */
    }
    for (size_t i = 0; i < pcnt; ++i)
        for (size_t j = i + 1; j < pcnt; ++j)
            sum += (seq_t)local_primes[i] * (seq_t)local_primes[j];   /* p_i * p_j */

    return sum;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* behaviour below offset irrelevant */

    init_spf();

    /* generate sequence values up to n */
    while (seq_len < (size_t)n) {
        ++current;               /* test next integer */
        seq_t val = compute_semiprime_sum(current);
        if (val != 0) {          /* at least two distinct semiprime divisors */
            if (seq_len >= MAX_TERMS) return 0; /* overflow guard */
            seq_vals[seq_len++] = val;
        }
    }
    return seq_vals[n - 1];
}
