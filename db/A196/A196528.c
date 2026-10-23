/*
 * A196528  Smallest number m such that A196527(m) = n.
 * https://oeis.org/A196528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A196528: smallest m such that
 *   gcd( sum of first m primes , sum of first m composites ) = n.
 * The implementation incrementally builds the primes and composites
 * and stores the resulting gcds.  For each queried n it scans the
 * pre‑computed range; if the target gcd is not yet found the range
 * is extended until it is discovered.  It uses only integer arithmetic
 * and the provided gcd() function. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

static seq_t      *gcds       = NULL;   /* gcd values for each m (1‑indexed). */
static size_t     capacity    = 0;       /* allocated size of gcds array. */
static size_t     max_m       = 0;       /* number of entries currently stored. */
static seq_t      sum_primes  = 0;       /* running sum of primes. */
static seq_t      sum_composites = 0;    /* running sum of composites. */
static idx_t      next_prime   = 2;      /* next candidate number to test for prime. */
static idx_t      next_composite = 4;   /* next candidate number to test for composite. */
static bool      initialized   = false;

/* Ensure that the gcds array can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= capacity) return;
    size_t newcap = capacity ? capacity * 2 : 16;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(gcds, newcap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    gcds = tmp;
    capacity = newcap;
}

/* Append one new entry (for m = max_m + 1) to the sequences. */
static void extend_one(void)
{
    /* Find next prime. */
    idx_t p = next_prime;
    while (!is_prime(p)) ++p;
    next_prime = p + 1;

    /* Find next composite (skipping primes and 1). */
    idx_t c = next_composite;
    while (is_prime(c) || c < 4) ++c;
    next_composite = c + 1;

    /* Update cumulative sums. */
    sum_primes      = sum_primes + (seq_t)p;
    sum_composites  = sum_composites + (seq_t)c;

    /* Compute the gcd. */
    seq_t g = gcd(sum_primes, sum_composites);

    /* Store in the array. */
    ensure_capacity(max_m + 1);
    gcds[max_m] = g;
    ++max_m;   /* now max_m entries are stored (index 0 .. max_m-1). */
}

/* Main requested function. */
seq_t a(idx_t n)
{
    /* Lazy initialization. */
    if (!initialized) {
        initialized = true;
        ensure_capacity(16);
    }

    seq_t target = (seq_t)n;

    /* Scan the already computed range. */
    for (size_t i = 0; i < max_m; ++i) {
        if (gcds[i] == target) return (seq_t)(i + 1);
    }

    /* Extend until we find the target gcd. */
    for (;;) {
        extend_one();
        if (gcds[max_m - 1] == target) return (seq_t)max_m;
    }
}
