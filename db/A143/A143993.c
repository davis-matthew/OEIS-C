/*
 * A143993  Composite numbers n such that the sum of prime factors of n
 *          (counted with multiplicity) terminates n as a substring.
 * https://oeis.org/A143993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth composite number such that the sum of its * prime
 *            factors (with multiplicity) equals the trailing digits of the *
 *            composite number
 * Range:     u8 n <= 5, u16 n <= 116, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, prime_factor_sum_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine uses a simple trial‑division factorisation for each
 * composite candidate.  The sequence is memoised in a static array
 * to allow arbitrary access order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/grow_seq.h"
#include "../utils/prime_factor_sum_uint64.h"

static inline bool is_composite(uint64_t x)
{
    if (x < 4) return false;          /* 0,1,2,3 are not composite */
    if (x % 2 == 0) return (x != 2);
    for (uint64_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) return true;
    }
    return false;
}

/* Number of decimal digits of an unsigned integer */
static inline int digits_uint64(uint64_t x)
{
    int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *terms   = NULL;  /* array of sequence values   */
    static size_t cap     = 0;     /* allocated capacity         */
    static idx_t count    = 0;     /* number of terms stored     */
    static uint64_t cand   = 4;    /* next candidate to test     */

    /* Return cached value if already computed */
    if (n > 0 && (idx_t)n <= count)
        return terms[(size_t)(n - 1)];

    /* Expand cache until we have at least n terms */
    while (count < n) {
        /* search for next composite satisfying the property  */
        for ( ; ; ++cand) {
            if (!is_composite(cand))
                continue;           /* skip non‑composites */

            /* sum of prime factors counted with multiplicity */
            seq_t sum = prime_factor_sum_uint64(cand);

            /* trailing digits test */
            int d     = digits_uint64((uint64_t)sum);
            seq_t pow10 = 1;
            for (int i = 0; i < d; ++i) pow10 *= 10;
            seq_t mod = (seq_t)cand % pow10;

            if (mod == sum)
                break;              /* found next term */
        }

        /* store the discovered term in the cache */
        if (count == (idx_t)cap)               /* capacity insufficient */
            grow_seq(&terms, &cap, (size_t)count + 1);

        terms[(size_t)count] = (seq_t)cand;
        ++count;
        ++cand;                                 /* move to next number */
    }

    return terms[(size_t)(n - 1)];
}
