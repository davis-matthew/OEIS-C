/*
 * A173655  Triangle read by rows: T(n,k) = prime(n) mod prime(k), 0 < k <= n.
 * https://oeis.org/A173655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = prime(r) mod prime(k) where r is the row containing the
 *            n-th element in the triangle T(r,k) = prime(r) mod prime(k) (k =
 *            1..r)
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n.
       We compute it by accumulating triangular numbers. */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* Column k (1-based) in row r */
    idx_t k = n - (sum - r);

    /* Lazy generation of primes up to the r-th prime */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (primes == NULL || r > prime_cnt) {
        /* Estimate an upper bound for the r-th prime.
           For r >= 6:  p(r) < r (log r + log log r)  (Rosser's theorem). */
        size_t needed = r;
        double rr = (double)needed;
        size_t limit;
        if (needed < 6)
            limit = 15;
        else
            limit = (size_t)(rr * (log(rr) + log(log(rr))) + 3.0);

        /* Allocate a fresh array of primes */
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes_upto(limit, &new_primes, &new_cnt);

        /* Release old storage if any */
        if (primes != NULL)
            free(primes);

        primes = new_primes;
        prime_cnt = new_cnt;
    }

    /* Convert the required primes to seq_t and compute the result */
    seq_t prime_r = (seq_t)primes[r - 1];
    seq_t prime_k = (seq_t)primes[k - 1];
    seq_t result = prime_r % prime_k;

    return result;
}
