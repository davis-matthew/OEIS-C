/*
 * A204921  a(n) = (p(n)^2 - q(n)^2)/n, where (p(n)^2, q(n)^2) is the least
 *          pair of squared primes (ordered as at A204914) for which n divides
 *          p(n)^2 - q(n)^2.
 * https://oeis.org/A204921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p(n)^2 - q(n)^2)/n where p(n) and q(n) are the least
 *            ordered pair of squared primes (A204914) for which n divides
 *            p(n)^2 - q(n)^2. Indexing starts at n = 1
 * Range:     u8 n <= 16, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Global prime storage */
static uint32_t *g_primes      = NULL;
static size_t   g_primes_count = 0;
static size_t   g_primes_cap   = 0;
static uint32_t g_next_candidate = 3;

/* Dynamically grow the prime array and add a new prime */
static void add_prime(uint32_t p)
{
    if (g_primes_count == g_primes_cap) {
        size_t new_cap = g_primes_cap ? g_primes_cap * 2 : 64;
        uint32_t *tmp = (uint32_t *)realloc(g_primes, new_cap * sizeof(uint32_t));
        if (!tmp) exit(1);          /* abort on allocation failure */
        g_primes       = tmp;
        g_primes_cap   = new_cap;
    }
    g_primes[g_primes_count++] = p;
}

/* Initialise the prime table with the first prime 2 */
static void init_primes(void)
{
    if (g_primes_count == 0) {
        add_prime(2);
        g_next_candidate = 3;
    }
}

/* Generate the next prime greater than the current candidate.
 * Primality test uses trial division by known primes up to sqrt. */
static uint32_t next_prime(void)
{
    for (;;) {
        uint32_t cand = g_next_candidate;
        bool ok = true;
        for (size_t i = 0; i < g_primes_count &&
             (uint64_t)g_primes[i] * g_primes[i] <= cand; ++i) {
            if (cand % g_primes[i] == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            add_prime(cand);
            g_next_candidate = cand + 2;
            return cand;
        }
        g_next_candidate += 2;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t nSeq = (seq_t)n;          /* convert index to seq_t once */
    size_t n_val = (size_t)n;       /* for array sizing (assumes n fits in size_t) */

    /* Allocate residue table, 0 indicates “none seen yet” */
    uint32_t *first_prime = (uint32_t *)calloc(n_val, sizeof(uint32_t));
    if (!first_prime) exit(1);       /* allocation failure */

    /* Ensure the prime list is initialized */
    init_primes();

    /* Iterate primes in increasing order */
    for (size_t ip = 0; ; ++ip) {
        if (ip >= g_primes_count)
            next_prime();            /* generate more primes if needed */

        uint32_t p = g_primes[ip];
        seq_t p2 = (seq_t)p * (seq_t)p;
        seq_t res = p2 % nSeq;       /* 0 <= res < n */

        uint32_t q = first_prime[(size_t)res];
        if (q != 0) {
            /* Found matching residue => desired pair */
            seq_t diff = p2 - (seq_t)q * (seq_t)q;
            free(first_prime);
            return diff / nSeq;
        } else {
            /* Record first prime for this residue */
            first_prime[(size_t)res] = p;
        }
    }
}
