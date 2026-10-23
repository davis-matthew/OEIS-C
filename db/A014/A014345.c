/*
 * A014345  Exponential convolution of primes with themselves.
 * https://oeis.org/A014345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} binomial(n,j)*prime(j+1)*prime(n-j+1)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 50, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Prime storage */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt  = 0;
static size_t   prime_cap  = 0;

/* Ensure that at least (need+1) primes are available, where
   need is a zero‑based index of the largest required prime.
   The primes are generated sequentially using trial division. */
static void ensure_primes(idx_t need)
{
    if (prime_cnt > need) return;

    /* Grow storage if necessary */
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    uint64_t candidate;
    if (prime_cnt == 0)
        candidate = 2;            /* first prime */
    else
        candidate = prime_list[prime_cnt - 1] + 1;  /* next odd candidate */

    while (prime_cnt <= need) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;  /* p > sqrt(candidate) */
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Ensure that primes up to index n exist */
    ensure_primes(n);

    seq_t result  = 0;   /* accumulator for a(n) */
    seq_t comb    = 1;   /* binomial(n,0) */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t p1 = (seq_t)prime_list[j];          /* prime(j+1) */
        seq_t p2 = (seq_t)prime_list[n - j];      /* prime(n-j+1) */
        seq_t term = p1 * p2;                     /* product of primes */

        result += comb * term;                    /* add weighted term */

        if (j < n) {
            /* Update binomial coefficient: C(n,j+1) = C(n,j)*(n-j)/(j+1) */
            seq_t numerator = comb * (seq_t)(n - j);
            comb = numerator / (seq_t)(j + 1);
        }
    }
    return result;
}
