/*
 * A143012  Numbers of the form (4^p + 2^p + 1)/7, where p > 3 is prime.
 * https://oeis.org/A143012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^p + 2^p + 1) / 7, where p is the n‑th prime greater
 *            than 3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

/* Cache for computed primes. */
static uint64_t *prime_buf = NULL;
static size_t   prime_cnt = 0;
static idx_t    prime_limit = 0;

/* Returns the n‑th term of sequence A143012. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Ensure we have at least n+2 primes (index 0..). */
    size_t required = (size_t)n + 2;
    if (prime_cnt < required) {
        /* Estimate an upper bound for the required prime using the prime number theorem. */
        idx_t new_limit;
        if (required < 6) {
            new_limit = 15;
        } else {
            double nn = (double)required;
            double estimate = nn * (log(nn) + log(log(nn))) + 3.0;
            new_limit = (idx_t)estimate + 10;
        }
        /* Increase limit if too small. */
        if (new_limit <= prime_limit) {
            new_limit = prime_limit * 2 + 100;
        }
        /* Rebuild the prime array. */
        if (prime_buf) free(prime_buf);
        prime_buf = NULL;
        prime_cnt = 0;
        build_primes_upto(new_limit, &prime_buf, &prime_cnt);
        prime_limit = new_limit;
    }

    /* Prime p is the (n+1)-th prime in the array (offset by 2 and 3). */
    uint64_t p = prime_buf[n + 1];

    /* Compute (4^p + 2^p + 1) / 7.
     * For small exponents a 128‑bit intermediate is safe; otherwise use big_pow.
     */
    seq_t result;
    if (p <= 63) {
        /* 4^p = 2^(2p), 2^p = 2^p.  Both fit in unsigned __int128 for p <= 63. */
        unsigned __int128 pow4 = 1;
        unsigned __int128 pow2 = 1;
        for (uint64_t i = 0; i < p; ++i) {
            pow4 *= 4;
            pow2 *= 2;
        }
        unsigned __int128 sum = pow4 + pow2 + 1;
        result = (seq_t)(sum / 7);
    } else {
        seq_t pow4 = big_pow((seq_t)4, (seq_t)p);
        seq_t pow2 = big_pow((seq_t)2, (seq_t)p);
        seq_t sum  = pow4 + pow2 + 1;
        result = sum / 7;
    }

    return result;
}
