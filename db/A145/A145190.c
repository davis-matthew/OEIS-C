/*
 * A145190  Numbers k such that phi(1)*phi(2)*...*phi(k) / k is an integer,
 *          where phi(k) is the totient function (A000010).
 * https://oeis.org/A145190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145190: a(n) is the n‑th integer k such that 
   (φ(1)*φ(2)*…*φ(k)) / k is an integer. 
   The function generates terms lazily and memoises them. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_K 500000            /* enough for n up to about 10000 */
#define MAX_FACTORS 10          /* maximum distinct primes in k */

static bool initialized = false;

static uint32_t spf[MAX_K + 1];          /* smallest prime factor */
static unsigned int exp_prod[MAX_K + 1]; /* exponents in product of φ */
static seq_t *terms = NULL;              /* memoised sequence terms */
static size_t terms_cap = 0;
static idx_t current_k = 0;              /* last k processed */
static idx_t current_n = 0;              /* number of terms stored */

/* ------------------------------------------------------------------ */
/* Sieve for smallest prime factors */
static void init_sieve(void)
{
    uint32_t i, j;
    for (i = 0; i <= MAX_K; ++i) spf[i] = 0;
    spf[0] = spf[1] = 1;
    for (i = 2; i * i <= MAX_K; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = i;
            for (j = i * i; j <= MAX_K; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    for (i = 2; i <= MAX_K; ++i)
        if (spf[i] == 0) spf[i] = i;      /* prime itself */
}

/* ------------------------------------------------------------------ */
/* Add exponents contributed by φ(n) to exp_prod */
static void factor_and_add(uint32_t n)
{
    while (n > 1) {
        uint32_t p = spf[n];
        unsigned int c = 0;
        while (n % p == 0) { n /= p; ++c; }
        exp_prod[p] += c;                /* add exponents of (p-1) */
    }
}

/* ------------------------------------------------------------------ */
/* Check whether k divides the product of φ(1)…φ(k) */
static bool check_divisibility(uint32_t k, const uint32_t pf[], const uint8_t pc[], int cnt)
{
    for (int i = 0; i < cnt; ++i)
        if (exp_prod[pf[i]] < pc[i]) return false;
    return true;
}

/* ------------------------------------------------------------------ */
/* Process next k, update exp_prod, test divisibility, store term */
static void process_next(void)
{
    uint32_t k = (uint32_t)++current_k;
    uint32_t pf[MAX_FACTORS];
    uint8_t  pc[MAX_FACTORS];
    int cnt = 0;

    /* factor k, store its prime factorization, update exp_prod */
    uint32_t n = k;
    while (n > 1) {
        uint32_t p = spf[n];
        unsigned int c = 0;
        while (n % p == 0) { n /= p; ++c; }

        pf[cnt] = p;
        pc[cnt] = (uint8_t)c;
        ++cnt;

        if (c > 1) exp_prod[p] += c - 1;   /* p^{c-1} from φ(p^c) */
        if (p > 2) factor_and_add(p - 1);  /* (p-1) factor of φ(p^c) */
    }

    /* test divisibility of k */
    if (check_divisibility(k, pf, pc, cnt)) {
        if (current_n >= terms_cap) {
            size_t new_cap = terms_cap == 0 ? 64 : terms_cap * 2;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            terms = tmp;
            terms_cap = new_cap;
        }
        terms[current_n++] = (seq_t)k;
    }
}

/* ------------------------------------------------------------------ */
/* Ensure at least n terms have been generated */
static void ensure_n_terms(idx_t n)
{
    while (current_n < n) process_next();
}

/* ------------------------------------------------------------------ */
/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */
    if (!initialized) {
        init_sieve();
        memset(exp_prod, 0, sizeof(exp_prod));
        initialized = true;
    }
    ensure_n_terms(n);
    return terms[n - 1];
}
