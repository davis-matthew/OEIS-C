/*
 * A092298  Number of partitions of the n-th prime into integers <= n.
 * https://oeis.org/A092298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula:
   a(n) = number of partitions of the n-th prime into integers <= n
   (OEIS A092298)
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* global prime list ----------------------------------------------------- */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* ensure at least n primes are stored: primes[0] = 2, primes[1] = 3, ... */
static void ensure_primes(size_t n)
{
    if (primes_cnt >= n) return;

    if (!primes) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) exit(1);
    }

    /* add the first prime 2 if not present */
    if (primes_cnt == 0) {
        primes[primes_cnt++] = 2ULL;
    }

    /* trial division to find subsequent primes */
    while (primes_cnt < n) {
        uint64_t cand = primes[primes_cnt - 1] + 1;
        if (cand > 2 && (cand & 1ULL) == 0) cand++;             /* skip even numbers */

        while (1) {
            bool isprime = true;
            for (size_t k = 0; k < primes_cnt; ++k) {
                uint64_t p = primes[k];
                if (p * p > cand) break;
                if (cand % p == 0) {
                    isprime = false;
                    break;
                }
            }
            if (isprime) {
                if (primes_cnt == primes_cap) {                  /* grow array */
                    size_t new_cap = primes_cap * 2;
                    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                    if (!tmp) exit(1);
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_cnt++] = cand;
                break;
            }
            cand += 2;  /* skip even numbers */
        }
    }
}

/* partition function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n < 1 */

    /* obtain the n-th prime */
    ensure_primes((size_t)n);
    size_t prime = (size_t)primes[n - 1];

    /* dynamic programming array */
    seq_t *dp = (seq_t *)malloc((prime + 1) * sizeof(seq_t));
    if (!dp) exit(1);

    memset(dp, 0, (prime + 1) * sizeof(seq_t));
    dp[0] = 1;                                 /* empty partition */

    /* build partitions using parts 1 .. n */
    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = i; j <= prime; ++j) {
            dp[j] += dp[j - i];
        }
    }

    seq_t result = dp[prime];
    free(dp);
    return result;
}
