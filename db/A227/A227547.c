/*
 * A227547  a(n) = a(n-1) + prime(n-1), with a(1)=3.
 * https://oeis.org/A227547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + prime(n-1), with a(1)=3
 * Range:     u8 n <= 14, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 3 + sum_{k=1}^{n-1} prime(k)          */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/next_prime.h"   /* prototype: uint64_t next_prime(uint64_t start) */

static uint64_t *prime_list = NULL;        /* stored primes */
static seq_t    *prime_sum  = NULL;        /* cumulative sums: prime_sum[i] = sum_{k=0}^i prime_list[k] */
static idx_t     prime_cnt  = 0;           /* number of primes stored */
static size_t    prime_cap  = 0;           /* allocated capacity */

/* Ensure at least 'need' primes are generated and summed */
static void
ensure_primes(idx_t need)
{
    if (need <= prime_cnt)
        return;

    if (prime_list == NULL) {
        prime_cap = 16;
        prime_list = (uint64_t*)malloc(sizeof(uint64_t) * prime_cap);
        prime_sum  = (seq_t*)malloc(sizeof(seq_t)      * prime_cap);
    }

    while (prime_cnt < need) {
        uint64_t p;
        if (prime_cnt == 0)
            p = 2;
        else
            p = next_prime(prime_list[prime_cnt - 1]);

        if (prime_cnt == prime_cap) {
            size_t new_cap = prime_cap * 2;
            uint64_t *tmp_list = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
            seq_t    *tmp_sum  = (seq_t*)realloc(prime_sum,  new_cap * sizeof(seq_t));
            if (!tmp_list || !tmp_sum) { /* allocation failed, abort gracefully */
                /* In an embedded environment we could handle this differently. */
                exit(EXIT_FAILURE);
            }
            prime_list = tmp_list;
            prime_sum  = tmp_sum;
            prime_cap  = new_cap;
        }

        prime_list[prime_cnt] = p;
        if (prime_cnt == 0)
            prime_sum[prime_cnt] = (seq_t)p;
        else
            prime_sum[prime_cnt] = prime_sum[prime_cnt - 1] + (seq_t)p;
        ++prime_cnt;
    }
}

/* Compute the n-th term of OEIS sequence A227547 */
seq_t
a(idx_t n)
{
    if (n == 0)   /* undefined; return 0 as a safe fallback */
        return 0;

    if (n == 1)
        return (seq_t)3;

    /* Need primes up to index n-1 (1-based) -> n-1 primes */
    ensure_primes(n - 1);               /* ensures prime_cnt >= n-1 */

    /* sum of first n-1 primes is prime_sum[n-2] */
    seq_t sum = prime_sum[(size_t)(n - 2)];
    return (seq_t)3 + sum;
}
