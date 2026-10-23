/*
 * A156311  Number of ways to write prime(n) = prime(k) + prime(k+1)*m.
 * https://oeis.org/A156311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k : prime(n) mod prime(k+1) = prime(k)}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, realloc, free, exit */
#include <stdbool.h>     /* bool, true, false */
#include <stddef.h>      /* size_t */

/* Prime list storage */
static uint64_t *prime_list = NULL; /* dynamic array of primes */
static size_t    prime_cnt   = 0;    /* number of primes stored */
static size_t    prime_cap   = 0;    /* capacity of prime_list */

/* Append the next prime after the current last prime.
   Uses trial division by all stored primes up to sqrt(candidate). */
static void add_next_prime(void)
{
    uint64_t cand = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;

    for (;;) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p > cand / p) break;                     /* p*p > cand */
            if (cand % p == 0) {                         /* composite */
                is_prime = false;
                break;
            }
        }
        if (is_prime) {                                 /* cand is prime */
            if (prime_cnt >= prime_cap) {                /* grow array */
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);            /* allocation failed */
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_cnt++] = cand;
            return;
        }
        ++cand;
    }
}

/* Ensure that prime_list contains at least n primes (n ≥ 1). */
static void ensure_n_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;
    while (prime_cnt < (size_t)n) add_next_prime();
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* undefined for n < 1, return 0 */

    ensure_n_primes(n);

    /* prime(n) */
    uint64_t p_n = prime_list[(size_t)(n - 1)];

    uint64_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        uint64_t pk  = prime_list[(size_t)(k - 1)];   /* prime(k)   */
        uint64_t pk1 = prime_list[(size_t)k];         /* prime(k+1) */
        if (p_n % pk1 == pk) ++count;
    }

    return (seq_t)count;
}
