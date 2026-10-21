/*
 * A069577  Smallest prime p such that pi(n) <= pi(p)*2, where pi(n) is the
 *          number of primes <= n, A000720.
 * https://oeis.org/A069577
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the (ceil(pi(n)/2))-th prime, where pi(n) is the number
 *            of primes <= n and the k-th prime is ordered by size (k starts
 *            at 1)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine relies on a simple incremental sieve that keeps a list of
   primes and a prefix array for pi(n).  For each call the sieve is
   extended as necessary.  The complexity per call is O(n log log n)
   for the largest n seen so far, easily fast enough for n up to 10 000. */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *prime_arr = NULL;    /* primes[0] == 2 */
static uint32_t *prime_pi  = NULL;    /* prefix counts: prime_pi[i] = pi(i) */
static size_t   prime_cnt = 0;        /* number of primes stored */
static idx_t    sieve_lim = 0;        /* current sieve upper bound */

/* Build or rebuild the sieve up to at least 'limit'.  All previous data
   is discarded and regenerated.  The function uses only integer
   arithmetic (no <math.h>). */
static void
build_sieve(idx_t limit)
{
    size_t sz = (size_t)(limit + 1);

    /* Allocate buffers.  If any allocation fails we simply abort – the
       function is called only from the sequence routine and an error
       termination is acceptable in the context of a library. */
    unsigned char *is_prime = (unsigned char*)malloc(sz);
    uint32_t      *pis      = (uint32_t*)malloc(sz * sizeof(uint32_t));
    uint32_t      *pr       = NULL;           /* size of this later */
    if (!is_prime || !pis) {
        free(is_prime); free(pis);
        return;
    }

    /* Mark all numbers as prime initially, then clear 0 and 1. */
    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    if (sz > 0) is_prime[0] = 0;
    if (sz > 1) is_prime[1] = 0;

    /* Classic sieve of Eratosthenes. */
    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes to allocate the prime array. */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++cnt;
    pr = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!pr) {
        free(is_prime); free(pis);
        return;
    }

    /* Build prime list and prefix counts. */
    size_t  pidx = 0;
    unsigned cumulative = 0;
    for (size_t i = 0; i <= (size_t)limit; ++i) {
        if (i >= 2 && is_prime[i]) {
            pr[pidx++] = (uint32_t)i;
            ++cumulative;
        }
        pis[i] = cumulative;
    }

    /* Replace old tables with the new ones. */
    free(prime_arr); free(prime_pi); free(is_prime);
    prime_arr = pr; prime_pi = pis; prime_cnt = cnt; sieve_lim = limit;
}

/* Sequence a(n).  It returns 0 for n < 2, which is outside the
   defined offset, but the behaviour for such arguments is irrelevant. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    if ((idx_t)sieve_lim < n)
        build_sieve((idx_t)n);     /* extend sieve as necessary */

    uint32_t pi_n = prime_pi[n];      /* number of primes <= n */
    idx_t    k    = ((idx_t)pi_n + 1) / 2;  /* ceil(pi(n)/2) */

    /* k is at least 1; the primes array is 0‑indexed. */
    return (seq_t)prime_arr[k - 1];
}
