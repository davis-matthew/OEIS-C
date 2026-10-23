/*
 * A101548  Number of k such that prime(n) divides the left factorial !k =
 *          sum_{i=0..k-1} i!.
 * https://oeis.org/A101548
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k such that prime(n) divides the left factorial
 *            !k where !k = sum_{i=0}^{k-1} i! (OEIS A101548)
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ----------------------------------------------------------
   Prime generation – up to the first 20000 primes
   ---------------------------------------------------------- */
#define MAX_PRIMES   20000          /* enough for n <= 10000 (the 10000th prime is 104729) */
#define SIEVE_LIMIT  300000         /* < 300000 contains at least 20000 primes */

static uint32_t primes[MAX_PRIMES];
static int prime_count = 0;

static void init_primes(void)
{
    if (prime_count > 0) return;                   /* already done */

    bool composite[SIEVE_LIMIT + 1];
    memset(composite, 0, sizeof(composite));

    for (uint32_t i = 2; i <= SIEVE_LIMIT && prime_count < MAX_PRIMES; ++i)
    {
        if (!composite[i])
        {
            primes[prime_count++] = i;
            if ((uint64_t)i * i <= SIEVE_LIMIT)
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    composite[j] = true;
        }
    }
}

/* ----------------------------------------------------------
   Count how many k (1 <= k <= p-1) satisfy p | !k
   ---------------------------------------------------------- */
static seq_t count_divisors(uint32_t p)
{
    seq_t cnt = 0;
    uint32_t fact = 1; /* 0! */
    uint32_t sum  = 0; /* no terms yet */

    for (uint32_t k = 1; k < p; ++k)
    {
        /* add (k-1)! to the cumulative sum, giving !k */
        sum += fact;
        if (sum >= p) sum -= p;               /* modulo p, fast */
        if (sum == 0) ++cnt;                  /* p divides !k */

        /* prepare fact = k!  for the next iteration */
        fact = (uint32_t)((uint64_t)fact * k % p);
    }
    return cnt;
}

/* ----------------------------------------------------------
   Storage for already computed a(n) values
   ---------------------------------------------------------- */
static seq_t *a_values = NULL;     /* array of size MAX_PRIMES+1, index 0..MAX_PRIMES */
static idx_t   computed_upto = 1;   /* highest n for which a(n) is known */

/* ----------------------------------------------------------
   Ensure that a_values[2..n] are computed
   ---------------------------------------------------------- */
static void ensure_computed(idx_t n)
{
    if (computed_upto >= n) return;

    if (a_values == NULL)
    {
        a_values = (seq_t*)calloc(MAX_PRIMES + 1, sizeof(seq_t));
        /* a_values[0] and a_values[1] stay zero (indexes < 2 are unused) */
    }

    init_primes();     /* guarantees primes[0..MAX_PRIMES-1] are ready */

    for (idx_t nn = computed_upto + 1; nn <= n; ++nn)
    {
        uint32_t p = primes[nn - 1];         /* prime(n) = primes[n-1] */
        a_values[nn] = count_divisors(p);
    }
    computed_upto = n;
}

/* ----------------------------------------------------------
   Public function required by the problem
   ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;             /* the sequence is defined for n >= 2 */

    ensure_computed(n);
    return a_values[n];
}
