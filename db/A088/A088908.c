/*
 * A088908  Numbers expressible as sum or difference of the squares of two
 *          distinct primes.
 * https://oeis.org/A088908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   cmps128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A088908: numbers expressible as ±(p² − q²) with distinct
   primes p,q.  The sequence is generated on demand, cached, and sorted
   once the required number of terms is requested. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/cmps128.h"

/* static cache */
static seq_t *cached_seq = NULL;   /* array of sequence values            */
static size_t cached_len  = 0;     /* number of values stored            */
static size_t cached_cap  = 0;     /* allocated capacity                  */
static uint64_t prime_limit = 1000;/* current upper bound for primes      */

/* generate enough terms to cover index n (1‑based) */
static void ensure_n_terms(idx_t n)
{
    while (cached_len < (size_t)n) {
        /* --------------------------------------------------
           1. Generate list of primes up to prime_limit
           -------------------------------------------------- */
        size_t m = prime_limit + 1;                     /* array size */
        char *is_prime = (char *)malloc(m);
        if (!is_prime) return;
        memset(is_prime, 1, m);
        is_prime[0] = is_prime[1] = 0;
        for (size_t p = 2; p * p < m; ++p)
            if (is_prime[p])
                for (size_t k = p * p; k < m; k += p)
                    is_prime[k] = 0;

        /* count primes */
        size_t pcnt = 0;
        for (size_t i = 2; i < m; ++i)
            if (is_prime[i]) ++pcnt;
        uint32_t *primes = (uint32_t *)malloc(pcnt * sizeof(uint32_t));
        if (!primes) { free(is_prime); return; }
        size_t idx = 0;
        for (size_t i = 2; i < m; ++i)
            if (is_prime[i]) primes[idx++] = (uint32_t)i;
        free(is_prime);

        /* --------------------------------------------------
           2. Generate all p²±q² for distinct primes p>q
           -------------------------------------------------- */
        size_t max_pairs = pcnt * (pcnt - 1) / 2;
        /* each pair yields two values: sum and difference */
        size_t tmp_cap = max_pairs * 2;
        unsigned __int128 *tmp = (unsigned __int128 *)malloc(tmp_cap * sizeof(unsigned __int128));
        if (!tmp) { free(primes); return; }
        size_t tcnt = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            unsigned __int128 p2 = (unsigned __int128)primes[i] * primes[i];
            for (size_t j = 0; j < i; ++j) {
                unsigned __int128 q2 = (unsigned __int128)primes[j] * primes[j];
                tmp[tcnt++] = p2 + q2;
                tmp[tcnt++] = p2 > q2 ? (p2 - q2) : (q2 - p2);
            }
        }
        free(primes);

        /* --------------------------------------------------
           3. Sort and de‑duplicate
           -------------------------------------------------- */
        qsort(tmp, tcnt, sizeof(unsigned __int128), cmps128);

        if (cached_cap < tcnt + 1) {
            cached_cap = tcnt + 1;
            cached_seq = (seq_t *)realloc(cached_seq, cached_cap * sizeof(seq_t));
            if (!cached_seq) { free(tmp); return; }
        }
        size_t pos = 0;
        for (size_t i = 0; i < tcnt; ++i) {
            if (i > 0 && tmp[i] == tmp[i-1]) continue; /* duplicate */
            uint64_t v64 = (uint64_t)tmp[i];
            cached_seq[pos++] = (seq_t)v64;
        }
        cached_len = pos;
        free(tmp);

        if (cached_len < (size_t)n) {
            /* not enough yet – increase prime bound and retry */
            prime_limit *= 2;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety; offset is 1 */
    ensure_n_terms(n);
    return cached_seq[(size_t)n - 1];
}
