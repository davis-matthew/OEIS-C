/*
 * A163366  a(n) = (-1)^floor((prime(n)+2)/2) mod prime(n).
 * https://oeis.org/A163366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^floor((prime(n)+2)/2) mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = 1 if prime(n) ≠ 2 and prime(n) ≠ 1 mod 4, else a(n)=prime(n)-1 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Dynamically build the list of primes as needed */
static idx_t *prime_list = NULL;      /* array of primes (uint64_t) */
static size_t prime_cnt  = 0;         /* number of primes stored */
static size_t prime_cap  = 0;         /* current capacity of prime_list */
static idx_t  last_checked = 1;       /* last integer tested for primality */

/* Ensure that at least n primes are available in prime_list */
static void ensure_n_primes(idx_t n)
{
    /* Expand the list until we have at least n primes */
    if (prime_cnt >= n) return;

    /* Initial allocation */
    if (prime_cap == 0) {
        prime_cap = 1024;
        prime_list = (idx_t *)malloc(prime_cap * sizeof(idx_t));
        if (!prime_list) exit(EXIT_FAILURE);
    }

    /* Simple incremental prime generation */
    while (prime_cnt < n) {
        ++last_checked;
        bool is_prime = true;
        idx_t r = (idx_t) sqrt((double)last_checked);   /* upper bound for trial divisions */

        for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
            if (last_checked % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Grow storage if necessary */
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = last_checked;
        }
    }
}

/* Compute the n-th term of sequence A163366 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, return 0 for safety */

    /* Make sure we have the requested prime */
    ensure_n_primes(n);

    idx_t p = prime_list[n - 1];    /* prime(n) */

    /* Special case for the prime 2 */
    if (p == 2) return (seq_t)1;

    /* For odd primes: return 1 if p ≡ 3 (mod 4), otherwise p−1 */
    if ((p % 4) == 1) {
        return (seq_t)(p - 1);
    } else {
        return (seq_t)1;
    }
}
