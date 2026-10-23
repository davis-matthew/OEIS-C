/*
 * A165981  Primes p which are equal to 2 plus the sum of three consecutive
 *          primes.
 * https://oeis.org/A165981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 395, u32 n <= 249, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Primes p that are equal to 2 plus the sum of three consecutive primes
    a(n) = the n-th prime p for which there exist consecutive primes
           q1, q2, q3 with p = 2 + q1 + q2 + q3.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* static state to allow out‑of‑order calls */
    static seq_t *primes      = NULL;   /* primes found so far          */
    static size_t   primes_cnt   = 0;    /* number of primes stored      */
    static size_t   primes_cap   = 0;    /* capacity of primes array     */

    static seq_t *results      = NULL;   /* sequence terms found         */
    static size_t  res_cnt      = 0;     /* number of terms stored       */
    static size_t  res_cap      = 0;     /* capacity of results array    */

    /* initialise once */
    if (results == NULL) {
        /* first prime is 2 */
        primes_cap = 16;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        if (!primes) exit(1);
        primes[primes_cnt++] = (seq_t)2;

        /* allocate space for sequence terms */
        res_cap = 32;
        results = (seq_t *)malloc(res_cap * sizeof(seq_t));
        if (!results) exit(1);
    }

    /* generate terms until the requested index is obtained */
    while (res_cnt < (size_t)n) {
        /* get next prime after the current last one */
        seq_t candidate = primes[primes_cnt - 1] + (seq_t)1;
        while (!is_prime_big(candidate)) candidate++;

        /* append the new prime, expanding array if needed */
        if (primes_cnt == primes_cap) {
            primes_cap <<= 1;
            primes = (seq_t *)realloc(primes, primes_cap * sizeof(seq_t));
            if (!primes) exit(1);
        }
        primes[primes_cnt++] = candidate;

        /* test the triple formed by the last three primes */
        if (primes_cnt >= 3) {
            seq_t sum = primes[primes_cnt - 3]
                      + primes[primes_cnt - 2]
                      + primes[primes_cnt - 1]
                      + (seq_t)2;
            if (is_prime_big(sum)) {
                if (res_cnt == res_cap) {
                    res_cap <<= 1;
                    results = (seq_t *)realloc(results,
                                                res_cap * sizeof(seq_t));
                    if (!results) exit(1);
                }
                results[res_cnt++] = sum;
            }
        }
    }

    return results[n - 1];
}
