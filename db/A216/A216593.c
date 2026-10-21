/*
 * A216593  Number of semiprimes among n-th million natural numbers.
 * https://oeis.org/A216593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of semiprimes between * 1 + (n-1)*10^6 and n*10^6
 *            (inclusive) * This implementation uses a segmented sieve and a
 *            prime table * generated once per call. It works for any n ≥ 1
 *            and can be * called in any order. The routine is O(10^6 log log
 *            n) per * call and uses ≤ 9 MB of static memory
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities provided by the build. */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **, size_t *); */
#include "../utils/int_sqrt.h"          /* idx_t int_sqrt(idx_t) */

#define BLOCK_SIZE 1000000ULL      /* size of each million block */

/* Counters are reused across calls (static storage). */
static uint64_t seg_rem[BLOCK_SIZE];
static unsigned char seg_cnt[BLOCK_SIZE];

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                  /* not defined for n = 0, return 0 */
    }

    /* start and end of the n‑th million block (inclusive) */
    const uint64_t start = (n - 1ULL) * BLOCK_SIZE + 1ULL;
    const uint64_t end   =  n        * BLOCK_SIZE;

    /* initialise segment arrays */
    for (idx_t i = 0; i < BLOCK_SIZE; ++i) {
        seg_rem[i] = start + i;
        seg_cnt[i] = 0;
    }

    /* generate primes up to sqrt(end) */
    const idx_t sqrt_end = int_sqrt(end);
    uint64_t *primes = NULL;
    size_t   prime_cnt = 0;
    build_primes_upto(sqrt_end, &primes, &prime_cnt);

    /* segmented sieve counting prime factors */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        /* first multiple of p inside the segment */
        uint64_t m = (start + p - 1) / p * p;
        for (; m <= end; m += p) {
            idx_t idx = m - start;
            while (seg_rem[idx] % p == 0) {
                seg_rem[idx] /= p;
                seg_cnt[idx] += 1;
            }
        }
    }

    /* handle remaining prime factor > sqrt(end) */
    for (idx_t i = 0; i < BLOCK_SIZE; ++i) {
        if (seg_rem[i] > 1ULL) {
            seg_cnt[i] += 1;
        }
    }

    /* count semiprimes (exactly two prime factors counted with multiplicity) */
    seq_t result = 0;
    for (idx_t i = 0; i < BLOCK_SIZE; ++i) {
        if (seg_cnt[i] == 2) {
            ++result;
        }
    }

    /* clean up allocated prime list */
    free(primes);

    return result;
}
