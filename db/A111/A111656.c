/*
 * A111656  n-th prime appears n-th composite number times.
 * https://oeis.org/A111656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cap_idx, ensure_cap_seq, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A111656:  each prime p_k appears c_k times where c_k is the
 * k-th composite number.  The implementation keeps a growing list of
 * primes, composites and the cumulative counts of composites.  For a
 * given index n it returns the prime that occupies the position n. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"
#include "../utils/ensure_cap_idx.h"
#include "../utils/ensure_cap_seq.h"

/* Test if x is prime using the primes that have already been found. */
static bool is_prime_local(idx_t x, const seq_t *primes, size_t m)
{
    if (x < 2) return false;
    idx_t limit = int_sqrt(x);
    for (size_t i = 0; i < m; ++i) {
        idx_t p = (idx_t)primes[i];
        if (p > limit) break;
        if (x % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* defined only for n ≥ 1 */

    /* static storage for successive calls --------------------------------- */
    static size_t m = 0;                 /* number of primes (and composites) stored */
    static seq_t  *primes = NULL;        /* primes[0..m-1]                */
    static size_t primes_cap = 0;
    static idx_t  *composites = NULL;    /* composites[0..m-1]            */
    static size_t composites_cap = 0;
    static idx_t  *cumulative = NULL;    /* cumulative[0..m]                */
    static size_t cumulative_cap = 0;
    static idx_t  prime_candidate = 2;   /* next number to test for prime   */
    static idx_t  composite_candidate = 4; /* next number to test for composite */

    /* Initialise the first entry (prime 2, composite 4). */
    if (m == 0) {
        ensure_cap_seq(1, &primes_cap, &primes);
        ensure_cap_idx(1, &composites_cap, &composites);
        ensure_cap_idx(2, &cumulative_cap, &cumulative);
        primes[0] = (seq_t)2;
        composites[0] = 4;
        cumulative[0] = 0;
        cumulative[1] = 4;    /* sum of first composite */
        m = 1;
        prime_candidate = 3;       /* next prime to test      */
        composite_candidate = 5;   /* next composite to test   */
    }

    /* Extend the data until we cover the required index n. */
    while (cumulative[m] < n) {
        /* Find next prime. */
        idx_t cand = prime_candidate;
        while (!is_prime_local(cand, primes, m)) ++cand;
        prime_candidate = cand + 1;
        seq_t prime_val = (seq_t)cand;

        /* Find next composite. */
        idx_t candi = composite_candidate;
        while (candi <= 1 || is_prime_local(candi, primes, m)) ++candi;
        composite_candidate = candi + 1;
        idx_t composite_val = candi;

        /* Grow the arrays if necessary. */
        ensure_cap_seq(m + 1, &primes_cap, &primes);
        ensure_cap_idx(m + 1, &composites_cap, &composites);
        ensure_cap_idx(m + 2, &cumulative_cap, &cumulative);

        primes[m] = prime_val;
        composites[m] = composite_val;
        cumulative[m + 1] = cumulative[m] + composite_val;
        ++m;
    }

    /* Locate the block that contains position n.  primes[0] corresponds
     * to cumulative[1], so we look for the smallest i with cumulative[i+1] ≥ n. */
    size_t i = 0;
    while (cumulative[i + 1] < n) ++i;

    return primes[i];
}
