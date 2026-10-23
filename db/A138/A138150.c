/*
 * A138150  n-th run has length n-th prime, with digits 0 and 1 only, starting
 *          with 0.
 * https://oeis.org/A138150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if the nth digit is in an odd‑indexed run (runs start
 *            with 0, run lengths are primes 2,3,5,…), 1 otherwise.
 *            Computation: generate primes until the cumulative sum of primes
 *            reaches n, then parity of the run index gives the digit
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* static cache of primes and cumulative sums */
static uint32_t *prime_cache = NULL;
static idx_t prime_cnt = 0;          /* number of primes stored */
static seq_t *cum_cache = NULL;
static idx_t cum_cnt = 0;            /* number of cumulative sums stored */

/* Ensure cumulative sums cover at least position N */
static void ensure_up_to_n(idx_t N)
{
    if (cum_cnt > 0 && cum_cache[cum_cnt - 1] >= (seq_t)N)
        return;

    uint32_t cand = prime_cnt ? prime_cache[prime_cnt - 1] + 1 : 2;

    /* generate primes until cumulative sum >= N */
    while (cum_cnt == 0 || cum_cache[cum_cnt - 1] < (seq_t)N) {
        /* find next prime */
        for (;;) {
            bool isprime = true;
            for (idx_t i = 0; i < prime_cnt && (uint64_t)prime_cache[i] * prime_cache[i] <= cand; ++i) {
                if (cand % prime_cache[i] == 0) {
                    isprime = false;
                    break;
                }
            }
            if (isprime) {
                /* store prime */
                prime_cache = (uint32_t *)realloc(prime_cache, (prime_cnt + 1) * sizeof(uint32_t));
                prime_cache[prime_cnt++] = cand;

                /* store cumulative sum */
                seq_t sum = (cum_cnt == 0) ? (seq_t)cand : cum_cache[cum_cnt - 1] + (seq_t)cand;
                cum_cache = (seq_t *)realloc(cum_cache, (cum_cnt + 1) * sizeof(seq_t));
                cum_cache[cum_cnt++] = sum;
                break;
            }
            ++cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for 0, but harmless */

    ensure_up_to_n(n);

    /* binary search for first cumulative sum >= n */
    idx_t low = 0, high = cum_cnt - 1, mid;
    while (low < high) {
        mid = (low + high) >> 1;
        if (cum_cache[mid] >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    /* run index is low+1 (1‑based) */
    return ((low + 1) & 1) ? 0 : 1;
}
