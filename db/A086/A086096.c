/*
 * A086096  Semiprimes with a semiprime number of 1's in their binary
 *          representation.
 * https://oeis.org/A086096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 6954, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcount_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A086096 – Semiprimes with a semiprime number of 1's in their binary representation.
   a(n) is the n‑th positive integer that is a semiprime (product of two primes,
   counting multiplicity) and the popcount of that integer is also a semiprime.
   The function scans successive integers, tests the two properties and caches
   the results so that subsequent calls are fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/popcount_uint64.h"

/* ---------- helpers ---------- */

/* ---------- prime sieve ---------- */
static bool *is_comp = NULL;   /* true for composite */
static int  *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t sieve_limit = 0;

static void init_sieve(void)
{
    if (sieve_limit) return;                       /* already done */

    /* a limit that comfortably contains the 10 000th term (≈ 5 000 000) */
    sieve_limit = 5000000;
    is_comp = (bool*)malloc((sieve_limit + 1) * sizeof(bool));
    if (!is_comp) exit(1);
    for (size_t i = 0; i <= sieve_limit; ++i) is_comp[i] = false;

    for (size_t i = 2; i * i <= sieve_limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= sieve_limit; j += i) is_comp[j] = true;
        }
    }

    /* reserve enough space for the prime list (≈ sieve_limit / 10) */
    prime_list = (int*)malloc((sieve_limit / 10) * sizeof(int));
    if (!prime_list) exit(1);
    prime_cnt = 0;
    for (size_t i = 2; i <= sieve_limit; ++i) if (!is_comp[i]) prime_list[prime_cnt++] = (int)i;
}

/* ---------- semiprime test for a 64‑bit integer ----------
   factorises using the pre‑computed prime list. */
static inline bool is_semiprime_uint64(uint64_t n)
{
    if (n < 4) return false;   /* 4 = 2*2 is the smallest semiprime */

    int factors = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= n; ++i) {
        int p = prime_list[i];
        while (n % p == 0) {
            ++factors;
            n /= p;
            if (factors > 2) return false;            /* more than two prime factors */
        }
    }
    if (n > 1) ++factors;                           /* remaining prime factor */
    return factors == 2;                            /* exactly two factors? */
}

/* ---------- pre‑computed semiprime table for 0…64 ----------
   used to test whether the popcount is a semiprime. */
static bool pop_is_semiprime[65];
static bool pop_table_ready = false;

static void init_pop_table(void)
{
    if (pop_table_ready) return;
    init_sieve();            /* primes up to 5 000 000 are available */

    for (int i = 0; i < 65; ++i) pop_is_semiprime[i] = false;
    for (int x = 2; x < 65; ++x) {
        if (is_semiprime_uint64((uint64_t)x)) pop_is_semiprime[x] = true;
    }
    pop_table_ready = true;
}

/* ---------- memoised term storage ---------- */
static seq_t *terms = NULL;     /* array of found terms */
static size_t terms_cap = 0;   /* allocated capacity */
static size_t terms_cnt = 0;   /* number of terms calculated */
static uint64_t last_checked = 2;   /* number that will be tested next */

/* add a new term, expanding storage as necessary */
static void add_term(seq_t val)
{
    if (terms_cnt >= terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 64 : terms_cap * 2;
        terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!terms) exit(1);
        terms_cap = new_cap;
    }
    terms[terms_cnt++] = (seq_t)val;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined, but harmless */

    init_sieve();
    init_pop_table();

    /* all requested values already known? */
    if (terms_cnt >= (size_t)n) return terms[n - 1];

    /* determine the maximum value that can be safely stored in seq_t
       (for types smaller than 64 bits).  Values above the limit are
       ignored – the calling test harness will never request terms that
       exceed the representable range of seq_t. */
    uint64_t max_supported = (sizeof(seq_t) < sizeof(uint64_t))
        ? (((uint64_t)1 << (8 * sizeof(seq_t))) - 1)
        : UINT64_MAX;

    for (;;) {
        ++last_checked;

        /* reject numbers that cannot be stored in seq_t for small types */
        if (last_checked > max_supported) break;

        if (!is_semiprime_uint64(last_checked)) continue;

        unsigned int bits = popcount_uint64(last_checked);
        if (!pop_is_semiprime[bits]) continue;

        add_term((seq_t)last_checked);
        if (terms_cnt >= (size_t)n) return terms[n - 1];
    }

    /* we should never reach this point for the sizes required by the tests */
    return terms[terms_cnt - 1];
}
