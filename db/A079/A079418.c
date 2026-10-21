/*
 * A079418  Numbers k such that prime(k)/k < prime(k-1)/(k-1).
 * https://oeis.org/A079418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that prime(k)/k < prime(k-1)/(k-1)
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>      /* for sqrt, used only in the prime test */

seq_t a(idx_t n)
{
    /* Static cache for primes and qualifying k's.  The function is able
       to be called in any order and will fill the cache lazily.        */

    static uint64_t *primes      = NULL;   /* list of primes seen so far    */
    static size_t   primes_cnt   = 0;       /* number of primes in the list  */
    static size_t   primes_cap   = 0;       /* allocated capacity for primes*/

    static seq_t   *goodks      = NULL;    /* list of k's that satisfy the inequality */
    static size_t  goodks_cnt   = 0;        /* number of stored k's            */
    static size_t  goodks_cap   = 0;        /* capacity of goodks array        */

    if (n == 0)  /* offset is 1; n>=1 is expected by the caller */
        return 0;

    /* Grow the list of qualifying k's until we have at least n of them. */
    while (goodks_cnt < n) {
        /* ----------------------------------------------------------------
         * 1. Produce the next prime.  We keep a list of all primes seen
         *    so far and test the candidate against that list.            */
        /* ---------------------------------------------------------------- */
        uint64_t cand;
        if (primes_cnt == 0)
            cand = 2;
        else
            cand = primes[primes_cnt - 1] + 1;

        /* Trial division using the primes that are already stored.  We
           stop when p*p > cand.  The product is tested using division
           to avoid overflow. */
        for (;;) {
            if (cand == 2) break;                                 /* 2 is prime */
            if ((cand & 1ULL) == 0) { ++cand; continue; }          /* skip evens */

            bool is_prime = true;
            for (size_t i = 0; i < primes_cnt; ++i) {
                uint64_t p = primes[i];
                if (p > cand / p) break;                          /* p*p > cand */
                if (cand % p == 0) { is_prime = false; break; }
            }
            if (is_prime) break;                                  /* prime found */
            ++cand;
        }

        /* Append the new prime into the list, extending the storage if needed. */
        if (primes_cnt == primes_cap) {
            size_t new_cap = primes_cap ? primes_cap * 2 : 16;
            uint64_t *tmp = (uint64_t *)realloc(primes,
                                                new_cap * sizeof(uint64_t));
            if (!tmp) abort(); /* out of memory */
            primes = tmp;
            primes_cap = new_cap;
        }
        primes[primes_cnt++] = cand;

        /* ----------------------------------------------------------------
         * 2.  Evaluate the inequality for the new index k = primes_cnt.  */
        /* ---------------------------------------------------------------- */
        if (primes_cnt >= 2) {
            uint64_t pk  = primes[primes_cnt - 1];
            uint64_t pkp = primes[primes_cnt - 2];
            idx_t kidx = (idx_t)primes_cnt;   /* k is 1‑based index of prime */

            /* Cross‑multiply to avoid division:
               pk/k < pkp/(k-1)  <=>  pk*(k-1) < pkp*k                */
            unsigned __int128 lhs = (unsigned __int128)pk  * (kidx - 1);
            unsigned __int128 rhs = (unsigned __int128)pkp * kidx;

            if (lhs < rhs) {
                /* Store the index k in the result list.             */
                if (goodks_cnt == goodks_cap) {
                    size_t new_cap = goodks_cap ? goodks_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(goodks,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort(); /* out of memory */
                    goodks = tmp;
                    goodks_cap = new_cap;
                }
                goodks[goodks_cnt++] = (seq_t)kidx;
            }
        }
    }

    /* Return the (n-1)-th element because goodks is 0‑based.           */
    return goodks[n - 1];
}
