/*
 * A238665  Primes p such that (p+3)^2+3 is prime but (p+j)^2+j is not prime
 *          for all 0<j<3.
 * https://oeis.org/A238665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that (p+3)^2+3 is prime but
 *            (p+1)^2+1 and (p+2)^2+2 are composite
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 1389, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;   /* array of found primes */
static size_t cap   = 0;      /* capacity of the array */
static size_t sz    = 0;      /* number of stored primes */
static uint64_t nextCandidate = 2; /* next integer to test */

/* Expand the cache to accommodate more results. */
static void grow_cache(void)
{
    size_t newCap = (cap == 0 ? 32 : cap << 1);
    seq_t *newCache;
    if (cache == NULL) {
        newCache = seq_alloc(newCap);
    } else {
        newCache = seq_realloc(cache, cap, newCap);
    }
    cache = newCache;
    cap   = newCap;
}

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; n>0 in normal use */

    /* Generate entries until we have at least n of them. */
    while (sz < n) {
        /* Skip even candidates greater than 2. */
        if (nextCandidate > 2 && (nextCandidate & 1ULL) == 0ULL) {
            ++nextCandidate;
            continue;
        }

        /* Treat the candidate as a seq_t value. */
        seq_t p = nextCandidate;

        if (isPrimeGeneric(p)) {
            seq_t v3 = p + 3;
            seq_t expr3 = v3 * v3 + 3;
            if (isPrimeGeneric(expr3)) {
                seq_t v1 = p + 1;
                seq_t expr1 = v1 * v1 + 1;
                seq_t v2 = p + 2;
                seq_t expr2 = v2 * v2 + 2;
                if (!isPrimeGeneric(expr1) && !isPrimeGeneric(expr2)) {
                    if (sz == cap) grow_cache();
                    cache[sz++] = p;
                }
            }
        }

        ++nextCandidate;
    }

    return cache[n - 1];
}
