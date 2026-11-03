/*
 * A106057  Primes p such that 1*p + 4 and 4*p + 1 are primes.
 * https://oeis.org/A106057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n‑th prime p such that p+4 and 4p+1 are also primes. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static seq_t *qual_primes = NULL;   /* array of qualifying primes   */
static idx_t  qual_cap   = 0;       /* current capacity              */
static idx_t  qual_cnt   = 0;       /* number of stored primes       */
static uint64_t next_candidate = 2; /* next integer to test for primality */

static void add_qual_prime(uint64_t p)
{
    if (qual_cnt == qual_cap) {
        size_t new_cap = qual_cap ? qual_cap << 1 : 8;
        seq_t *tmp = mem_resize(seq_t, qual_primes, new_cap);
        if (!tmp) exit(1); /* allocation failed */
        qual_primes = tmp;
        qual_cap    = new_cap;
    }
    qual_primes[qual_cnt++] = (seq_t)p;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, but safe fallback */

    /* initialise storage if needed */
    if (qual_primes == NULL) {
        qual_cap = 8;
        qual_primes = mem_new(seq_t, qual_cap);
        if (!qual_primes) exit(1);
    }

    while (qual_cnt < n) {
        /* find next prime */
        do { ++next_candidate; } while (!is_prime_u64(next_candidate));

        /* test the required conditions */
        uint64_t p = next_candidate;
        if (is_prime_u64(p + 4) && is_prime_u64(4ULL * p + 1)) {
            add_qual_prime(p);
        }
    }

    return qual_primes[n - 1];
}
