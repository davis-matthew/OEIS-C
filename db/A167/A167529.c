/*
 * A167529  a(n) is the number of nonisolated nonprimes k such that (n-th
 *          nonisolated prime) < k < (n-th isolated prime).
 * https://oeis.org/A167529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of composite integers k such that p_non < k <
 *            p_iso, where p_non is the n-th nonisolated prime and p_iso is
 *            the n-th isolated prime. A composite k is counted iff it is NOT
 *            the middle of a twin‑prime pair (i.e., not of the form p+1 with
 *            primes p and p+2)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   build_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes.h"

static idx_t *primes = NULL;          /* all primes up to current_limit       */
static size_t primes_cnt = 0;
static idx_t *isolated_primes = NULL; /* isolated primes only                  */
static size_t isolated_cnt = 0;
static idx_t *nonisolated_primes = NULL; /* nonisolated primes only          */
static size_t nonisolated_cnt = 0;
static uint64_t current_limit = 0;

/* Rebuild all arrays up to 'limit' (inclusive).  The arrays are
   automatically freed if memory is reclaimed from a previous build. */
static void build_up_to(uint64_t limit)
{
    idx_t *tmp_primes = NULL;
    size_t tmp_cnt = 0;

    /* Generate the list of all primes ≤ limit. */
    build_primes(limit, &tmp_primes, &tmp_cnt);

    /* Classify each prime as isolated or nonisolated. */
    idx_t *tmp_iso = (idx_t*)malloc(tmp_cnt * sizeof(idx_t));
    idx_t *tmp_noniso = (idx_t*)malloc(tmp_cnt * sizeof(idx_t));
    size_t iso_c = 0, noniso_c = 0;

    for (size_t i = 0; i < tmp_cnt; ++i) {
        idx_t p = tmp_primes[i];
        if (is_prime(p-2) || is_prime(p+2))
            tmp_noniso[noniso_c++] = p;
        else
            tmp_iso[iso_c++] = p;
    }

    /* Replace the old tables. */
    free(primes);
    free(isolated_primes);
    free(nonisolated_primes);

    primes = tmp_primes;
    primes_cnt = tmp_cnt;
    isolated_primes = tmp_iso;
    isolated_cnt = iso_c;
    nonisolated_primes = tmp_noniso;
    nonisolated_cnt = noniso_c;
    current_limit = limit;
}

/* Ensure that we have at least 'n' isolated and 'n' nonisolated primes. */
static void ensure_primes(idx_t n)
{
    if (primes == NULL) {                /* first time */
        build_up_to(1000);
    }
    while (isolated_cnt < n || nonisolated_cnt < n) {
        uint64_t new_limit = current_limit * 2 + 100;
        build_up_to(new_limit);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n=0 */

    ensure_primes(n);

    size_t idx = (size_t)(n - 1);
    idx_t low_prime   = nonisolated_primes[idx];
    idx_t high_prime  = isolated_primes[idx];

    if (low_prime >= high_prime)
        return (seq_t)0;                 /* empty interval */

    uint64_t low  = (uint64_t)low_prime  + 1;
    uint64_t high = (uint64_t)high_prime - 1;

    uint64_t cnt = 0;
    for (uint64_t k = low; k <= high; ++k) {
        if (is_prime(k))
            continue;                    /* prime, skip */

        /* Exclude composites that are the middle of a twin‑prime pair. */
        if (is_prime(k-1) && is_prime(k+1))
            continue;

        ++cnt;
    }

    return (seq_t)cnt;
}
