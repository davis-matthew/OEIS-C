/*
 * A138672  a(n) = prime(n)^3 mod prime(n-1).
 * https://oeis.org/A138672
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^3 mod prime(n-1) (for n ≥ 2)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for generated primes */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;
static idx_t  next_cand   = 3; /* next candidate to test for primality */

/* Ensure that prime_list contains at least 'need' primes */
static void ensure_primes(idx_t need)
{
    if (prime_list == NULL) {
        /* initial allocation */
        prime_cap   = 64;
        prime_list  = (idx_t *)malloc(prime_cap * sizeof(idx_t));
        if (!prime_list) exit(1);
        /* first prime */
        prime_list[0] = 2;
        prime_count   = 1;
        next_cand     = 3;
    }

    while (prime_count < (size_t)need) {
        /* find the next prime, starting from next_cand (odd numbers) */
        for ( ; ; ) {
            idx_t cand = next_cand;
            next_cand += 2;                /* next odd number */

            bool is_prime = true;
            for (size_t i = 0; i < prime_count; ++i) {
                idx_t p = prime_list[i];
                if (p * p > cand) break;
                if (cand % p == 0) { is_prime = false; break; }
            }

            if (is_prime) {
                /* store new prime */
                if (prime_count == prime_cap) {
                    prime_cap = prime_cap * 2 + 16;
                    idx_t *tmp = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
                    if (!tmp) exit(1);
                    prime_list = tmp;
                }
                prime_list[prime_count++] = cand;
                break;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0; /* undefined for n < 2, return 0 as placeholder */

    /* make sure we have primes up to index n */
    ensure_primes(n);

    idx_t p_n   = prime_list[n - 1]; /* prime(n) */
    idx_t p_prev = prime_list[n - 2]; /* prime(n-1) */

    seq_t p   = (seq_t)p_n;
    seq_t mod = (seq_t)p_prev;

    /* compute p^3 mod mod */
    seq_t temp = (p * p) % mod;
    seq_t res  = (temp * p) % mod;

    return res;
}
