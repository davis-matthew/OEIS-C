/*
 * A163505  a(n) = (n-th odd nonprime) mod (n-th odd number).
 * https://oeis.org/A163505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-th odd nonprime) mod (n-th odd number)
 * Range:     u8 n <= 104, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*
 * The function computes A163505.  It keeps a static cache of the
 * odd non‑primes found so far and a list of primes used for
 * fast primality testing.  The computation is incremental – for
 * each call it extends the cache until it contains the n‑th
 * odd non‑prime.  The n‑th odd number is simply 2*n‑1.
 */
seq_t a(idx_t n)
{
    /* Offset 1 – if the caller uses 0 return 0 */
    if (n == 0) return 0;

    /* static cache of odd non‑primes */
    static seq_t *nonprimes = NULL;
    static idx_t np_cap = 0;
    static idx_t np_len = 0;

    /* static list of primes for trial division */
    static seq_t *primes = NULL;
    static idx_t pr_cap = 0;
    static idx_t pr_len = 0;

    /* the next odd number to test */
    static seq_t next_odd = 1;
    static bool initialized = false;

    if (!initialized) {
        /* initialise caches */
        np_cap = 8;
        nonprimes = (seq_t *)malloc(np_cap * sizeof(seq_t));
        if (!nonprimes) abort();

        pr_cap = 8;
        primes = (seq_t *)malloc(pr_cap * sizeof(seq_t));
        if (!primes) abort();

        initialized = true;
    }

    while (np_len < n) {
        seq_t m = next_odd;
        bool is_prime = false;

        if (m == 1) {
            /* 1 is not prime */
            is_prime = false;
        } else {
            /* test primality using the found primes */
            is_prime = true;
            for (idx_t i = 0; i < pr_len; ++i) {
                seq_t p = primes[i];
                seq_t sq = p * p;
                if (sq > m) break;
                if (m % p == 0) {
                    is_prime = false;
                    break;
                }
            }
        }

        if (!is_prime) {
            /* store the odd non‑prime */
            if (np_len == np_cap) {
                np_cap = np_cap * 2;
                seq_t *tmp = (seq_t *)realloc(nonprimes,
                                              np_cap * sizeof(seq_t));
                if (!tmp) abort();
                nonprimes = tmp;
            }
            nonprimes[np_len++] = m;
        }

        if (is_prime) {
            /* store the prime for future checks */
            if (pr_len == pr_cap) {
                pr_cap = pr_cap * 2;
                seq_t *tmp = (seq_t *)realloc(primes,
                                              pr_cap * sizeof(seq_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[pr_len++] = m;
        }

        next_odd += 2;
    }

    /* n-th odd number is 2*n-1 */
    seq_t odd_n = (seq_t)2 * n - 1;
    seq_t val   = nonprimes[n - 1] % odd_n;
    return val;
}
