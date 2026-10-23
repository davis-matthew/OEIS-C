/*
 * A195086  Numbers k such that (number of prime factors of k counted with
 *          multiplicity) less (number of distinct prime factors of k) = 2.
 * https://oeis.org/A195086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that the sum of prime exponents minus the number
 *            of distinct prime factors equals 2 (Ω(k) – ω(k) = 2)
 * Range:     u8 n <= 25, u16 n <= 6313, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Prime sieve: generate all primes up to a fixed limit.              */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL;   /* dynamic array of primes */
static size_t   prime_cnt  = 0;       /* number of primes stored   */
static bool     primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;

    /* limit large enough for all numbers we will test */
    const uint64_t SIEVE_LIMIT = 2000000U;   /* 2 * 10^6  */
    bool *is_composite = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_composite) abort();

    for (uint64_t p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (!is_composite[p]) {
            for (uint64_t q = p * p; q <= SIEVE_LIMIT; q += p)
                is_composite[q] = true;
        }
    }

    /* count primes first */
    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_composite[i]) ++prime_cnt;

    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_composite[i]) prime_list[idx++] = i;

    free(is_composite);
    primes_ready = true;
}

/* ------------------------------------------------------------------ */
/* Helper: compute sum_{i}(e_i - 1) for the prime factorisation of k.*/
/* Return 0,1,2 or 3 (3 means >2).                                  */
/* ------------------------------------------------------------------ */
static int omega_minus_omega_star(uint64_t k)
{
    init_primes();   /* make sure prime list is ready */

    uint64_t temp = k;
    int diff = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > temp / p) break;           /* p*p > temp */
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            diff += e - 1;
            if (diff > 2) return 3;        /* early exit */
        }
    }

    /* remaining prime factor (if any) has exponent 1 -> contributes 0 */
    return diff;
}

/* ------------------------------------------------------------------ */
/* Sequence storage                                         */
/* ------------------------------------------------------------------ */
static seq_t *seq     = NULL;   /* array of sequence terms             */
static idx_t  seq_len = 0;      /* number of stored terms              */
static idx_t  seq_cap = 0;      /* current capacity                     */
static uint64_t last_checked = 0; /* last integer examined */

/* ------------------------------------------------------------------ */
/* Main function                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* 1-indexed: a(1) is the first term.  For n < 1 behaviour is
       irrelevant (offset = 1).                                    */
    if (n < 1) return (seq_t)0;

    /* lazy allocation for sequence table */
    if (seq_cap == 0) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
    }

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        uint64_t k = ++last_checked;

        if (omega_minus_omega_star(k) == 2) {
            /* append k to the sequence */
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) abort();
            }
            seq[seq_len++] = (seq_t)k;   /* cast safe: k fits in seq_t */
        }
    }

    return seq[n - 1];
}
