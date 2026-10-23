/*
 * A109027  Numbers that have exactly seven prime factors counted with
 *          multiplicity (A046308) whose digit reversal is different and also
 *          has 7 prime factors (with multiplicity).
 * https://oeis.org/A109027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega_uint64, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A109027: Numbers with exactly seven prime factors (counted with multiplicity)
    whose digit reversal is different and also has seven prime factors.  */
/*  Implementation:  generate all numbers up to a fixed limit with a linear
    sieve that records Ω(n).  Scan sequentially, test the reversal, and
    cache results so that any call to a(n) (for n ≥ 1) returns immediately
    once the sequence has been built up to that index.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega_uint64.h"
#include "../utils/rev.h"

#define LIMIT 35000000U  /* search limit – gives >10 000 terms */

static uint32_t *spf = NULL;      /* smallest prime factor array */
static uint8_t  *bigomega = NULL; /* Ω(n) values for 1..LIMIT */

static seq_t  *terms = NULL;      /* cached sequence values */
static size_t   terms_size = 0;   /* number of stored terms   */
static size_t   terms_cap  = 0;   /* capacity of terms array */

static size_t   max_checked = 0;  /* largest integer that has been examined */

static void init_sieve(void)
{
    size_t N = LIMIT;
    spf = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    if (!spf) exit(1);
    bigomega = (uint8_t *)malloc((N + 1) * sizeof(uint8_t));
    if (!bigomega) exit(1);

    /* temporary array for the list of primes */
    size_t max_primes = N / 10;          /* over‑estimate, still < 4 MB */
    uint32_t *primes = (uint32_t *)malloc(max_primes * sizeof(uint32_t));
    if (!primes) exit(1);
    size_t primes_cnt = 0;

    spf[0] = spf[1] = 0;
    bigomega[0] = bigomega[1] = 0;

    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {              /* i is prime */
            spf[i] = (uint32_t)i;
            bigomega[i] = 1;
            if (primes_cnt == max_primes) {
                max_primes *= 2;
                primes = (uint32_t *)realloc(primes, max_primes * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t v = (uint64_t)i * p;
            if (v > N) break;
            spf[(size_t)v] = p;
            bigomega[(size_t)v] = bigomega[i] + 1;
            if (p == spf[i]) break;
        }
    }
    free(primes);
}

static void push_term(seq_t val)
{
    if (terms_size == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 64;
        seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        terms = new_arr;
        terms_cap = new_cap;
    }
    terms[terms_size++] = val;
}

static void compute_until(idx_t n)
{
    if (terms_size >= (size_t)n) return;

    if (!spf) init_sieve();

    size_t cur = (max_checked == 0) ? 1 : max_checked + 1;
    while (terms_size < (size_t)n) {
        seq_t val     = (seq_t)cur;
        seq_t omega_val;
        if (cur <= LIMIT) {
            omega_val = (seq_t)bigomega[cur];
        } else {
            omega_val = bigomega_uint64((uint64_t)cur);
        }

        if (omega_val == 7) {
            seq_t rev_val = rev(val);
            if (rev_val != val) {
                seq_t omega_rev = bigomega_uint64((uint64_t)rev_val);
                if (omega_rev == 7) {
                    push_term(val);
                }
            }
        }
        ++cur;
    }
    max_checked = cur - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset 1 – behaviour for n = 0 is irrelevant */
    compute_until(n);
    return terms[n - 1];
}
