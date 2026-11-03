/*
 * A136207  Primes p such that p-6 or p+6 is prime.
 * https://oeis.org/A136207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 3097, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9603 (2026-11-01)
 * Helpers:   is_prime, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A136207:
   a(n) = the n-th prime p such that at least one of p-6 or p+6 is prime.
   Indexing starts at 1: a(1)=5.  The algorithm generates primes up
   to a sufficient limit by sieving, then collects the wanted primes,
   caching the result for future calls.  When the cached list is
   insufficient the sieve limit is doubled and the process repeats. */

/* Required environment interfaces */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/isqrt.h"

/* Ensure that an array of idx_t has at least 'need' slots. */
static void ensure_idx_t_capacity(idx_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(*arr, new_cap * sizeof(idx_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Local helpers ------------------------------------------------------- */

/* Simple Eratosthenes sieve up to 'limit' (inclusive).  The primes
   are stored in 'primes', whose capacity is expanded as needed.      */
static void generate_primes(idx_t limit, idx_t **primes, size_t *pcnt, size_t *pcap)
{
    /* Allocate a boolean array for the sieve. */
    bool *isprime = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!isprime) return;                /* memory failure – unlikely */

    for (idx_t i = 2; i <= limit; ++i) isprime[(size_t)i] = true;

    idx_t root = isqrt(limit);
    for (idx_t i = 2; i <= root; ++i) {
        if (isprime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                isprime[(size_t)j] = false;
        }
    }

    for (idx_t i = 2; i <= limit; ++i) {
        if (isprime[(size_t)i]) {
            ensure_idx_t_capacity(primes, pcap, *pcnt + 1);
            (*primes)[(*pcnt)++] = i;
        }
    }
    free(isprime);
}

/* Collect primes p for which p-6 or p+6 is prime into 'special'.        */
static void collect_special(const idx_t *primes, size_t pcnt,
                            idx_t **special, size_t *spcnt, size_t *spcap)
{
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p = primes[i];
        bool ok = false;
        if (p > 6 && is_prime(p - 6)) ok = true;
        else if (is_prime(p + 6)) ok = true;
        if (ok) {
            ensure_idx_t_capacity(special, spcap, *spcnt + 1);
            (*special)[(*spcnt)++] = p;
        }
    }
}

/* Main function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static state for caching */
    static bool initialized = false;
    static idx_t limit = 0;
    static idx_t *primes = NULL;    size_t pcnt = 0, pcap = 0;
    static idx_t *special = NULL;   size_t spcnt = 0, spcap = 0;

    if (!initialized) {
        /* Start with a modest sieve limit that comfortably covers n up to 10k. */
        limit = 6000000;            /* ≈ 6 M; enough for the first ~10k special primes */

        /* Loop until we have at least n special primes */
        while (true) {
            /* Generate all primes up to 'limit'. */
            pcnt = pcap = 0;
            free(primes);
            primes = NULL;
            generate_primes(limit, &primes, &pcnt, &pcap);

            /* Collect the special primes into 'special'. */
            spcnt = spcap = 0;
            free(special);
            special = NULL;
            collect_special(primes, pcnt, &special, &spcnt, &spcap);

            if (spcnt >= n)
                break; /* sufficient data */

            /* Not enough candidates – enlarge the sieve limit. */
            limit <<= 1;   /* double the limit */
        }
        initialized = true;
    }

    /* n is 1‑based; special[] is 0‑based. */
    return (seq_t)special[n - 1];
}
