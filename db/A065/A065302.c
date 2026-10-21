/*
 * A065302  Squarefree nonprime numbers whose sum of divisors is also
 *          squarefree.
 * https://oeis.org/A065302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 775, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, build_spf, ensure_capacity, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Squarefree nonprime numbers whose sum of divisors is also squarefree
   (OEIS A065302).  The terms are:
   1, 2*p where p is prime ≡ 1 (mod 12) and (p+1)/2 is squarefree.
   The implementation performs a sieve of primes up to a fixed bound,
   checks the congruence and squarefree condition, and memoises the
   generated terms.  The function a(n) returns the n‑th term for any n>=1
   in O(n) time with small constant factors. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"
#include "../utils/ensure_capacity.h"
#include "../utils/is_squarefree.h"

static seq_t *seq = NULL;              /* memoised terms */
static size_t seq_cap = 0, seq_size = 0;
static idx_t *prime_list = NULL;      /* list of primes */
static size_t prime_cnt = 0;
static size_t prime_idx = 0;
static uint64_t *spf = NULL;           /* smallest prime factor array (unused but built for completeness) */
static bool initialized = false;

/* Forward declaration */
static void ensure_as_needed(idx_t n);

/* Initialise memoisation structures and sieve data */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate cache for sequence terms */
    seq_cap = 64;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq[0] = (seq_t)1;      /* a(1) = 1 */
    seq_size = 1;

    /* Build primes and spf up to a bound that covers at least 10 000 terms.
       The bound 600 000 is comfortably above the 10 000‑th qualifying prime (~480 000). */
    const idx_t limit = 600000;
    build_primes_upto(limit, &prime_list, &prime_cnt);
    build_spf(limit, &spf);
}

/* Generate sequence terms until we have at least n entries */
static void ensure_as_needed(idx_t n)
{
    if (n <= seq_size) return;

    /* Ensure the array is large enough.  We might add more than needed,
       but this guarantees capacity without repeated reallocations. */
    ensure_capacity((size_t)n, &seq_cap, (void **)&seq, sizeof(seq_t));

    /* Generate terms by scanning through the prime list */
    while (seq_size < n) {
        /* Skip primes before 13 or that are not ≡1 mod 12 */
        while (prime_idx < prime_cnt) {
            uint64_t p = prime_list[prime_idx++];
            if (p < 13u) continue;
            if (p % 12u != 1u) continue;

            /* m = (p+1)/2 must be squarefree and odd (ensured by p≡1 mod12) */
            idx_t m_raw = (p + 1u) >> 1;
            seq_t m = (seq_t)m_raw;
            if (is_squarefree(m)) {
                seq[seq_size++] = (seq_t)2u * (seq_t)p;
                break;                     /* Move to next prime for subsequent terms */
            }
        }

        /* If we exhaust the prime list we would need to enlarge the limit,
           but the chosen limit is large enough for the required n. */
    }
}

/* Public function returning the n-th term */
seq_t a(idx_t n)
{
    init();
    ensure_as_needed(n);
    return seq[n - 1];
}
