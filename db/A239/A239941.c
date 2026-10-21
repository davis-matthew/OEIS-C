/*
 * A239941  Primes p which are floor of Root-mean-cube (RMC) of prime(n),
 *          prime(n+1) and prime(n+2).
 * https://oeis.org/A239941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 1745,
 *            u128 n <= 1745, big any n
 * Helpers:   append_candidate, build_primes_upto, floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239941: a(n) is the nth prime that equals
   floor( sqrt( (p³ + q³ + r³)/3 ) ) for some consecutive primes
   p, q, r.  The function generates primes on the fly, caching
   results so it works for repeated, out‑of‑order queries. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/is_prime_big.h"       /* is_prime_big */
#include "../utils/append_candidate.h"   /* append_candidate */
#include "../utils/floor_sqrt.h"         /* floor_sqrt */

/* Cached sequence values */
static seq_t *seq_cache   = NULL;
static size_t seq_len     = 0;
static size_t seq_cap     = 0;

/* Cached list of primes */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static size_t   tri_index   = 0;   /* first index of current triplet */

/* Initialise the global data on first use */
static void init_globals(void)
{
    static bool done = false;
    if (done) return;

    /* A safe upper bound that provides enough primes for n ≤ 10 000 */
    const idx_t PRIME_LIMIT = 2000000;   /* pi(2 000 000) ≈ 148 933 */

    build_primes_upto(PRIME_LIMIT, &prime_list, &prime_cnt);
    tri_index = 0;

    seq_cache = NULL;
    seq_len   = 0;
    seq_cap   = 0;

    done = true;
}

/* Append a new candidate to the cached sequence */
static inline void add_term(seq_t val)
{
    append_candidate(&seq_cache, &seq_len, &seq_cap, val);
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety: offset starts at 1 */

    init_globals();

    /* Generate terms until the cached list contains at least n */
    while (seq_len < (size_t)n) {

        /* Ensure there are three primes available */
        if (tri_index + 2 >= prime_cnt) {
            /* This should not happen for the intended limits */
            return 0;
        }

        uint64_t p0 = prime_list[tri_index];
        uint64_t p1 = prime_list[tri_index + 1];
        uint64_t p2 = prime_list[tri_index + 2];

        /* Compute (p0³ + p1³ + p2³) / 3  using seq_t arithmetic */
        seq_t sum  = ((seq_t)p0 * p0 * p0)
                   + ((seq_t)p1 * p1 * p1)
                   + ((seq_t)p2 * p2 * p2);
        seq_t mean = sum / 3;

        /* Floor square root of the mean */
        seq_t candidate = floor_sqrt(mean);

        /* If the candidate itself is prime, store it */
        if (is_prime_big(candidate))
            add_term(candidate);

        /* Move to the next overlapping triplet */
        ++tri_index;
    }

    return seq_cache[n - 1];   /* a(1) is stored at index 0 */
}
