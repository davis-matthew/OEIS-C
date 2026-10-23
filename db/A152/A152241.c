/*
 * A152241  Products of cubes of 2 successive primes.
 * https://oeis.org/A152241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_n * p_{n+1})^3 where p_k is the k‑th prime (1‑based)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------- */
/*   Prime cache (simple incremental sieve)    */
/* ------------------------------------------- */
static uint64_t *prime_arr = NULL;     /* array of generated primes   */
static size_t     prime_cnt = 0;       /* number of primes in array   */
static size_t     prime_cap = 0;       /* allocated capacity          */

/* Ensure that prime_arr contains primes up to at least index 'upto'.
   Indexing is 0‑based; this creates primes[0] .. primes[upto]. */
static void ensure_primes(size_t upto)
{
    if (prime_cnt > upto)
        return;                      /* already enough                 */

    /* initial allocation */
    if (prime_cap == 0) {
        prime_cap  = 16;
        prime_arr  = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_arr) exit(1);     /* out of memory, abort          */
    }

    /* seed with the first two primes */
    if (prime_cnt == 0) {
        prime_arr[prime_cnt++] = 2;
        prime_arr[prime_cnt++] = 3;
        if (prime_cnt > upto)
            return;
    }

    uint64_t candidate = prime_arr[prime_cnt-1] + 2;  /* next odd number */
    while (prime_cnt <= upto) {
        bool is_prime = true;

        /* trial division by known primes */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_arr[i];
            if (p*p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_arr,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                prime_arr = tmp;
            }
            prime_arr[prime_cnt++] = candidate;
        }
        candidate += 2;   /* next odd candidate */
    }
}

/* ------------------------------------------- */
/*   OEIS sequence implementation             */
/* ------------------------------------------- */
seq_t a(idx_t n)
{
    /* n is 1‑based; we need primes[n-1] and primes[n] (0‑based). */
    size_t target_index = (size_t)n;   /* last needed index   */
    ensure_primes(target_index);

    uint64_t p1 = prime_arr[(size_t)n - 1];
    uint64_t p2 = prime_arr[(size_t)n];

    seq_t prod = (seq_t)p1 * (seq_t)p2;
    seq_t cube = prod * prod * prod;
    return cube;
}
