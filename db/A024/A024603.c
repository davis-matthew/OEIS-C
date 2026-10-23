/*
 * A024603  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (odd natural numbers), t = (primes).
 * https://oeis.org/A024603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} (2*i-1) * p_{n+1-i}
 * where p_k is the k‑th prime (2,3,5,…).
 *
 * Computes the OEIS sequence A024603.
 * Uses a dynamic sieve to maintain a cache of primes sufficient
 * for the largest n requested so far.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* cache of the first primes, zero‑based: prime_cache[0] = 2 */
static uint64_t *prime_cache = NULL;
static size_t   prime_cnt   = 0;     /* number of primes cached */

/* ensure that prime_cache holds at least n primes */
static void
ensure_primes_upto_n(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;                         /* already sufficient */

    idx_t limit = n * 10 + 100;          /* initial guess */
    while (1) {
        uint64_t *tmp  = NULL;
        size_t    tmpc = 0;
        build_primes_upto((idx_t)limit, &tmp, &tmpc);
        if (tmpc >= (size_t)n) {
            free(prime_cache);           /* replace old cache */
            prime_cache = tmp;
            prime_cnt   = tmpc;
            break;
        }
        free(tmp);                       /* not enough, double limit */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)       /* undefined, but guard against accidental call */
        return 0;

    ensure_primes_upto_n(n);             /* guarantees prime_cnt >= n */

    idx_t  k   = (n + 1) / 2;
    seq_t  sum = 0;

    for (idx_t i = 1; i <= k; ++i) {
        idx_t prime_index = n - i;                /* zero‑based index */
        seq_t prime_val   = (seq_t)prime_cache[prime_index];
        seq_t odd_factor  = (seq_t)(2 * i - 1);
        sum += odd_factor * prime_val;
    }

    return sum;
}
