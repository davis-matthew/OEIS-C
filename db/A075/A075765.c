/*
 * A075765  a(n) = floor(prime(n)/n) + (prime(n) mod n).
 * https://oeis.org/A075765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(prime(n)/n) + (prime(n) mod n) (OEIS A075765)
 * Range:     u8 n <= 54, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Get the n-th prime, generating further primes as needed. */
static seq_t *prime_list = NULL;  /* array of primes (stored as seq_t)    */
static idx_t  prime_cnt  = 0;     /* number of primes stored in prime_list */
static idx_t  prime_cap  = 0;     /* capacity of prime_list               */
static idx_t  candidate   = 2;    /* current candidate number             */

/* Ensure that prime_cnt >= needed.  Generate primes using trial division. */
static void ensure_primes(idx_t needed)
{
    /* Expand array capacity as necessary */
    if (prime_cap == 0) {
        prime_cap = 4;
        prime_list = (seq_t *) malloc(prime_cap * sizeof(seq_t));
    } else if (prime_cnt >= prime_cap) {
        prime_cap <<= 1; /* double the capacity */
        prime_list = (seq_t *) realloc(prime_list, prime_cap * sizeof(seq_t));
    }

    while (prime_cnt < needed) {
        bool is_prime = true;
        idx_t sq = (idx_t) sqrt((double) candidate);
        for (idx_t i = 0; i < prime_cnt && prime_list[i] <= sq; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_list[prime_cnt++] = (seq_t) candidate;
        }
        ++candidate;
    }
}

/* Public API: compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) {   /* undefined, but return 0 for safety */
        return 0;
    }

    ensure_primes(n);
    seq_t p   = prime_list[n - 1];
    seq_t k   = p / (seq_t)n;
    seq_t r   = p % (seq_t)n;
    return k + r;
}
