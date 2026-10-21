/*
 * A058529  Numbers whose prime factors are all congruent to +1 or -1 modulo
 *          8.
 * https://oeis.org/A058529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = all positive integers whose prime factors are all
 *            congruent to +1 or –1 modulo 8. The algorithm generates the
 *            sequence by a sieve that marks multiples of forbidden primes (2
 *            and primes ≡3,5 (mod 8)) up to a growing upper bound until at
 *            least n good numbers have been found
 * Range:     u8 n <= 29, u16 n <= 5584, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* start with a moderate bound */
    idx_t limit = 2 * n;
    if (limit < 20) limit = 20;

    for (;;) {
        /* allocate boolean arrays */
        size_t sz = (size_t)limit + 1;
        unsigned char *good   = (unsigned char*)malloc(sz);
        unsigned char *isPrime = (unsigned char*)malloc(sz);
        if (!good || !isPrime) {
            if (good) free(good);
            if (isPrime) free(isPrime);
            return (seq_t)0;          /* out of memory – should not happen */
        }

        /* initialise arrays */
        memset(good, 1, sz);          /* good[i] == true initially  */
        good[0] = 0;                  /* 0 is not in the sequence */
        memset(isPrime, 1, sz);
        isPrime[0] = isPrime[1] = 0;  /* 0,1 are not primes */

        /* sieving primes up to limit and marking forbidden multiples */
        for (uint64_t i = 2; i <= limit; ++i) {
            if (isPrime[(size_t)i]) {
                /*  mark multiples of i as composite for the prime sieve   */
                if (i * i <= (uint64_t)limit) {
                    for (uint64_t j = i * i; j <= (uint64_t)limit; j += i)
                        isPrime[(size_t)j] = 0;
                }

                /*  handle the prime i for the sequence sieve            */
                if (i == 2) {                 /* 2 is forbidden */
                    for (uint64_t m = 2; m <= (uint64_t)limit; m += 2)
                        good[(size_t)m] = 0;
                } else if (i % 8 == 3 || i % 8 == 5) {
                    /* primes ≡3 or 5 (mod 8) are forbidden */
                    for (uint64_t m = i; m <= (uint64_t)limit; m += i)
                        good[(size_t)m] = 0;
                }
            }
        }

        /* count good numbers and check if we have enough */
        idx_t count = 0;
        for (idx_t i = 1; i <= limit; ++i)
            if (good[(size_t)i]) ++count;

        if (count >= n) {
            /* find the n‑th good number */
            idx_t idx = 0;
            seq_t  result = 0;
            for (idx_t i = 1; i <= limit; ++i) {
                if (good[(size_t)i]) {
                    ++idx;
                    if (idx == n) {
                        result = (seq_t)i;
                        break;
                    }
                }
            }
            free(good);
            free(isPrime);
            return result;
        }

        /* not enough – enlarge the bound and try again */
        free(good);
        free(isPrime);
        limit *= 2;
    }
}
