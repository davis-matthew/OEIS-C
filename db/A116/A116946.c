/*
 * A116946  Closest semiprime to n-th semiprime S that is different from S
 *          (break ties by taking the smaller semiprime).
 * https://oeis.org/A116946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: For the n-th semiprime S, return the
 * closest other semiprime to S (break ties by choosing the smaller). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static cache of found semiprimes (ascending). */
static seq_t *semiprimes = NULL;
static size_t semiprime_count  = 0;
static size_t semiprime_cap    = 0;
static uint64_t last_checked   = 1;   /* highest integer examined for semiprime */

/* Append a semiprime to the cache, expanding storage if necessary. */
static void append_semiprime(seq_t val)
{
    if (semiprime_count == semiprime_cap) {
        size_t new_cap = semiprime_cap ? semiprime_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(semiprimes, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        semiprimes = tmp;
        semiprime_cap = new_cap;
    }
    semiprimes[semiprime_count++] = val;
}

/* Ensure that at least n_needed semiprimes are cached. */
static void build_semiprimes_upto(idx_t n_needed)
{
    if (semiprime_count >= (size_t)n_needed)
        return;   /* already have enough */

    uint64_t limit = last_checked + 1;
    if (limit < 2) limit = 2;

    while (semiprime_count < (size_t)n_needed) {
        /* Expand limit for the next sieve. */
        limit = limit * 2;
        if (limit < last_checked + 1) limit = last_checked + 1;

        /* Smallest prime factor array. */
        uint64_t *spf = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
        if (!spf) abort();

        /* Linear sieve to fill spf. */
        uint64_t *primes = NULL;
        size_t pcap = 0, pcnt = 0;
        for (uint64_t i = 2; i <= limit; ++i) {
            if (spf[i] == 0) {          /* i is prime */
                spf[i] = i;
                if (pcnt == pcap) {
                    pcap = pcap ? pcap * 2 : 4;
                    uint64_t *tmp = (uint64_t *)realloc(primes, pcap * sizeof(uint64_t));
                    if (!tmp) abort();
                    primes = tmp;
                }
                primes[pcnt++] = i;
            }
            for (size_t k = 0; k < pcnt && primes[k] <= spf[i] && i * primes[k] <= limit; ++k) {
                spf[i * primes[k]] = primes[k];
            }
        }

        /* Scan the new range for semiprimes. */
        for (uint64_t x = last_checked + 1; x <= limit; ++x) {
            uint64_t p = spf[x];
            if (p == 0) continue;
            uint64_t q = x / p;
            if (q < 2) continue;
            if (p == q) {
                /* Square of a prime. */
                if (spf[p] == p)
                    append_semiprime((seq_t)x);
            } else {
                /* Product of two distinct primes. */
                if (spf[p] == p && spf[q] == q)
                    append_semiprime((seq_t)x);
            }
        }

        last_checked = limit;
        free(spf);
        free(primes);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset 1; n=0 is undefined but return 0 safely */
        return 0;

    /* Need at least one semiprime before and after n-th to choose a neighbour. */
    build_semiprimes_upto(n + 1);

    seq_t target = semiprimes[n - 1];   /* n-th semiprime */

    seq_t best = 0;
    seq_t bestdiff = 0;
    bool has_best = false;

    /* Candidate from the previous semiprime. */
    if (n > 1) {
        seq_t prev = semiprimes[n - 2];
        seq_t diff = target > prev ? target - prev : prev - target;
        best = prev;
        bestdiff = diff;
        has_best = true;
    }

    /* Candidate from the next semiprime. */
    if (n < semiprime_count) {
        seq_t next = semiprimes[n];
        seq_t diff = next > target ? next - target : target - next;
        if (!has_best || diff < bestdiff || (diff == bestdiff && next < best)) {
            best = next;
            bestdiff = diff;
            has_best = true;
        }
    }

    return best;
}
