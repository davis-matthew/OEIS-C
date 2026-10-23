/*
 * A201264  Primes of the form 9n^3+2.
 * https://oeis.org/A201264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A201264: primes of the form 9k³+2
   (k starts at 0).  The function returns the n-th such prime.
   It scans k sequentially, testing primality by means of the
   provided is_prime_seq helper, and caches results so repeated
   calls are fast.  The implementation works for any size of
   seq_t (8, 16, 32, 64, 128 bits or arbitrary precision). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"

static seq_t cache[10000];    /* store at most the first 10000 terms */
static idx_t cache_count = 0;
static idx_t next_k = 0;      /* next k to test (k >= 0) */

seq_t a(idx_t n)
{
    /* Ensure cache is big enough (problem guarantees n ≤ 10000). */
    while (cache_count < n) {
        seq_t val;
        for (;;) {
            /* val = 9*k^3 + 2, computed in seq_t */
            val = (seq_t)9 * (seq_t)next_k
                  * (seq_t)next_k * (seq_t)next_k
                  + (seq_t)2;
            ++next_k;
            if (is_prime_seq(val))
                break;
        }
        cache[cache_count++] = val;
    }
    return cache[n - 1];
}
