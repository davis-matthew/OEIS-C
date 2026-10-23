/*
 * A196226  m such that A054024(m) (sum of divisors of m reduced modulo m) is
 *          3 + m/2.
 * https://oeis.org/A196226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A196226:
   a(1) = 8;
   a(n) = 2 * prime_{n+1}   for n ≥ 2
   where prime_k is the k‑th prime (prime_1 = 2, prime_2 = 3, …).
   Thus the sequence consists of 8 followed by twice each prime > 3. */
/* Dependencies: <stdbool.h>, <stddef.h>, <stdint.h>, <stdlib.h>, <string.h>
   (these are allowed by the build environment) */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static bool sieve_done = false;
static idx_t *primes = NULL;
static size_t primes_len = 0;

/* Build a sieve of Eratosthenes up to `limit` and store all primes in `primes`. */
static void build_primes_up_to(idx_t limit)
{
    /* Mark composites */
    bool *composite = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!composite) exit(1);
    memset(composite, 0, (size_t)(limit + 1) * sizeof(bool));

    /* Rough upper bound for number of primes up to `limit` (~ limit / log(limit)) */
    size_t est = (size_t)(limit / 10 + 10);
    idx_t *temp = (idx_t*)malloc(est * sizeof(idx_t));
    if (!temp) exit(1);
    size_t cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!composite[i]) {
            if (cnt == est) {          /* grow temporary buffer on demand */
                est *= 2;
                idx_t *newtmp = (idx_t*)realloc(temp, est * sizeof(idx_t));
                if (!newtmp) exit(1);
                temp = newtmp;
            }
            temp[cnt++] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    composite[j] = true;
            }
        }
    }

    /* Store primes in the global array */
    free(primes);
    primes = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!primes) exit(1);
    memcpy(primes, temp, cnt * sizeof(idx_t));
    primes_len = cnt;

    free(temp);
    free(composite);
    sieve_done = true;
}

/* Ensure that we have at least `need` primes (primes[0] = 2). */
static void ensure_primes(size_t need)
{
    if (sieve_done && primes_len >= need) return;

    /* A simple upper bound for the n-th prime: p_n < n * (log n + log log n)
       Here we use a rough multiplier to keep the code simple. */
    idx_t limit = (idx_t)(need * 20);
    if (limit < 100) limit = 100;
    build_primes_up_to(limit);

    /* If still not enough (rare for small n), grow the sieve */
    while (primes_len < need) {
        limit *= 2;
        build_primes_up_to(limit);
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)8;          /* first term is 8 */

    /* We need primes[n] (since primes[0]=2 gives prime_1).  For n>=2
       this corresponds to prime_{n+1}. */
    ensure_primes((size_t)n + 1);

    seq_t val = (seq_t)primes[(size_t)n];
    val *= (seq_t)2;
    return val;
}
