/*
 * A163976  prime(n) -(-1)^(n-th nonprime).
 * https://oeis.org/A163976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - (-1)^(nonprime(n)) Here nonprime(1) = 0,
 *            nonprime(2) = 1, ... For even nonprime we subtract 1, for odd
 *            nonprime we add 1
 * Range:     u8 n <= 54, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static idx_t *prime_list    = NULL;   /* prime_list[i] = (i+1)-th prime    */
static size_t prime_len    = 0;        /* number of primes stored           */
static idx_t *nonprime_list = NULL;   /* nonprime_list[i] = (i+1)-th nonprime  */
static size_t nonprime_len = 0;        /* number of non‑primes stored       */
static idx_t next_candidate = 0;       /* next integer to classify         */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for out‑of‑range calls   */

    /* Build tables until we have at least n entries each. */
    while (prime_len < (size_t)n || nonprime_len < (size_t)n) {
        if (is_prime(next_candidate)) {    /* prime */
            idx_t *tmp = (idx_t *)realloc(prime_list,
                                          (prime_len + 1) * sizeof(idx_t));
            if (!tmp) abort();
            prime_list = tmp;
            prime_list[prime_len++] = next_candidate;
        } else {                            /* nonprime (0,1,4,6,...) */
            idx_t *tmp = (idx_t *)realloc(nonprime_list,
                                          (nonprime_len + 1) * sizeof(idx_t));
            if (!tmp) abort();
            nonprime_list = tmp;
            nonprime_list[nonprime_len++] = next_candidate;
        }
        ++next_candidate;
    }

    seq_t p = (seq_t)prime_list[n - 1];
    /* parity of the n-th nonprime decides the sign */
    if (nonprime_list[n - 1] & (idx_t)1)   /* odd -> add 1 */
        return p + (seq_t)1;
    else                                 /* even -> subtract 1 */
        return p - (seq_t)1;
}
