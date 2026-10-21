/*
 * A088684  Prime(3n+3)-prime(3n+1).
 * https://oeis.org/A088684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = prime(3n+3) - prime(3n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A088684: difference between the (3n+3)rd
 * and the (3n+1)st primes.  For n up to 10 000 the needed prime
 * index is 3n+3 ≤ 30 003, so we sieve all primes up to 400 000,
 * which safely contains the first 31 000 primes. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* External function required by the framework */
seq_t a(idx_t n)
{
    /* Static state to keep a single prime list between calls */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Build all primes up to 400 000.  This contains over
         * 31 000 primes, enough for prime indices up to 30 003. */
        size_t limit = 400000;
        build_primes_upto(limit, &primes, &prime_cnt);
        initialized = true;
    }

    /* Compute prime indices.  n is idx_t, 0‑based offset for the
     * sequence is 1, so prime(3n+3) refers to primes[3n+2] and
     * prime(3n+1) refers to primes[3n] (0‑based). */
    idx_t idx_big = 3 * n + 3;
    idx_t idx_small = 3 * n + 1;

    /* Safety: ensure indices are within bounds.  For the typical
     * limits of this sequence the assertion will never fail. */
    if (idx_big == 0 || idx_small == 0 ||
        idx_big > prime_cnt || idx_small > prime_cnt) {
        /* Out of bounds – for safety return 0.  The sequence
         * definition guarantees this will never happen for
         * valid inputs. */
        return (seq_t)0;
    }

    /* Compute the difference as seq_t. */
    seq_t diff = (seq_t)(primes[idx_big - 1] - primes[idx_small - 1]);
    return diff;
}
