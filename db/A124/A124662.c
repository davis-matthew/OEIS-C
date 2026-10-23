/*
 * A124662  Primes prime(n) such that there exists a 0 < k < n-1 such that
 *          prime(n+k) + prime(n-k) = 2*prime(n).
 * https://oeis.org/A124662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A124662
   a(n) = the n-th prime p_n such that there exists an integer k,
          0 < k < n-1, with prime(n+k) + prime(n-k) = 2*prime(n). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state ----------------------------------------------- */
    static seq_t *primes = NULL;          /* all primes found          */
    static size_t  primes_len = 0;        /* number of primes stored  */
    static size_t  primes_cap = 0;        /* allocated capacity       */
    static uint8_t *tested = NULL;        /* whether prime at idx tested yet */
    static size_t  next_test = 0;         /* smallest index not yet tested */

    static seq_t *good = NULL;            /* primes that satisfy condition   */
    static size_t  good_len = 0;          /* how many good primes stored     */
    static size_t  good_cap = 0;          /* capacity of good array          */

    static uint64_t next_candidate = 0;   /* next odd integer to test       */

    /* initialise first prime(s) --------------------------------- */
    if (primes_len == 0) {
        primes_cap = 16;
        good_cap   = 16;
        primes = (seq_t*)malloc(primes_cap * sizeof(seq_t));
        tested = (uint8_t*)malloc(primes_cap * sizeof(uint8_t));
        good   = (seq_t*)malloc(good_cap   * sizeof(seq_t));

        /* add prime 2 */
        primes[primes_len++] = (seq_t)2;
        tested[0] = 0;                 /* not tested yet */
        next_candidate = 3;             /* next odd to check */
        next_test = 0;                  /* start testing from the beginning */
    }

    /* generate enough good primes -------------------------------- */
    while (good_len < n) {
        /* ------------------------------------------------------- */
        /* find the next prime and append it to `primes`.         */
        /* ------------------------------------------------------- */
        while (1) {
            uint64_t cand = next_candidate;
            bool is_prime = true;

            for (size_t pi = 0; pi < primes_len; ++pi) {
                uint64_t p = (uint64_t)primes[pi];
                if (p * p > cand) break;
                if (cand % p == 0) { is_prime = false; break; }
            }

            next_candidate += 2;          /* advance to next odd number */

            if (is_prime) {
                /* ensure capacity for new prime */
                if (primes_len >= primes_cap) {
                    primes_cap *= 2;
                    primes = (seq_t*)realloc(primes,
                         primes_cap * sizeof(seq_t));
                    tested = (uint8_t*)realloc(tested,
                         primes_cap * sizeof(uint8_t));
                }
                primes[primes_len++] = (seq_t)cand;
                tested[primes_len-1] = 0;  /* not tested yet */

                /* ------------------------------------------------- */
                /* test all earlier primes that now have sufficient  */
                /* future primes to evaluate the condition.          */
                /* ------------------------------------------------- */
                while (next_test < primes_len &&
                       2*(next_test+1)-1 <= primes_len) {
                    size_t idx = next_test;          /* 0‑based index of prime p_n */
                    seq_t center = primes[idx];
                    seq_t double_center = center + center;

                    bool good_flag = false;
                    for (size_t k = 1; k < idx+1; ++k) {
                        seq_t left  = primes[idx - k];
                        seq_t right = primes[idx + k];
                        if (left + right == double_center) {
                            good_flag = true;
                            break;
                        }
                    }
                    if (good_flag) {
                        if (good_len >= good_cap) {
                            good_cap *= 2;
                            good = (seq_t*)realloc(good,
                                  good_cap * sizeof(seq_t));
                        }
                        good[good_len++] = center;
                    }
                    ++next_test;
                }

                /* exit the candidate loop if we already have enough */
                if (good_len >= n) break;
                /* otherwise generate the next candidate prime   */
                break;
            }
            /* if not prime, continue to next odd candidate      */
        }
    }

    return good[n-1];
}
