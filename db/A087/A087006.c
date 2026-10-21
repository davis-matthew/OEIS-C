/*
 * A087006  Divisors of 30030.
 * https://oeis.org/A087006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A087006 - Divisors of 30030 in ascending order
   Formula: a(n) equals the n-th divisor (1-indexed) of 30030=2*3*5*7*11*13.
   For n>64 (the total number of divisors) a(n) returns 0. */

/* The function must be callable with any n >= 1, in any order.  The
   divisors are generated once (static) and sorted; subsequent calls
   simply index into the array. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper comparator for qsort on seq_t values.
   seq_t may be any unsigned integer type (including __int128). */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Number of prime factors of 30030 */
    static const uint64_t primes[6] = {2,3,5,7,11,13};
    static const size_t PF = 6;           /* number of primes   */
    static seq_t divisors[64];            /* there are 2^6 = 64 divisors */
    static size_t div_count = 0;          /* computed after init */
    static int initialized = 0;           /* guard for one-time init */

    if (!initialized) {
        /* generate all subsets of the 6 primes */
        div_count = 0;
        for (uint64_t mask = 0; mask < (1ULL << PF); ++mask) {
            seq_t prod = 1;
            for (size_t i = 0; i < PF; ++i) {
                if (mask & (1ULL << i))
                    prod *= (seq_t)primes[i];
            }
            divisors[div_count++] = prod;
        }
        /* sort the divisor list */
        qsort(divisors, div_count, sizeof(seq_t), cmp_seq_t);
        initialized = 1;
    }

    if (n == 0 || n > div_count)   /* n must be >=1, but guard anyway */
        return (seq_t)0;
    else
        return divisors[n-1];       /* a(1) is the first divisor */
}
