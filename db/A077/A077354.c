/*
 * A077354  a(n) = Sum_{i=n+1..2n} prime(i) - Sum_{i=1..n} prime(i).
 * https://oeis.org/A077354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=n+1..2n} prime(i) - Sum_{i=1..n} prime(i)
 *            (prime(i) counts the i‑th prime, starting with prime(1)=2)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/sieve_primes.h"   /* void sieve_primes(idx_t limit, idx_t **primes_out, size_t *cnt_out); */

static idx_t  max_prepared = 0;      /* largest n for which tables are built */
static uint64_t *prime_list = NULL; /* list of primes up to current limit */
static size_t   prime_cnt   = 0;     /* number of primes stored */
static seq_t   *cumulative  = NULL; /* cumulative sums: cumulative[i] = sum of first i primes */

/* Ensure data structures have at least 2*n primes.
   Uses a crude upper bound: n*20 for n>10, else 200. */
static void prepare(idx_t n)
{
    if (n <= max_prepared) return;      /* already prepared */

    /* free previous allocations */
    free(prime_list);
    free(cumulative);
    prime_list = NULL;
    cumulative = NULL;
    prime_cnt   = 0;
    max_prepared = 0;

    /* estimate upper bound on 2*n‑th prime */
    idx_t limit = 200;
    if (n > 10) limit = n * 20;
    while (true) {
        uint64_t *p = NULL;
        size_t   c = 0;
        sieve_primes(limit, &p, &c);
        if (c >= (size_t)(2 * n)) {   /* enough primes */
            prime_list = p;
            prime_cnt  = c;
            break;
        }
        free(p);                      /* not enough, increase limit */
        limit <<= 1;
    }

    /* build cumulative sums */
    cumulative = (seq_t *)malloc((prime_cnt + 1) * sizeof(seq_t));
    cumulative[0] = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
        cumulative[i + 1] = cumulative[i] + (seq_t)prime_list[i];

    max_prepared = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* defined only for n>=1 */

    prepare(n);                            /* ensure tables cover 2*n primes */

    /* safety check – recompute if bound was insufficient */
    if (prime_cnt < (size_t)(2 * n))
        prepare(n);

    seq_t sum_n = cumulative[(size_t)n];
    /* a(n) = Σ_{i=n+1}^{2n} p_i - Σ_{i=1}^{n} p_i
              = cumulative[2n] - 2 * cumulative[n]                     */
    seq_t result = cumulative[(size_t)(2 * n)] - sum_n - sum_n;
    return result;
}
