/*
 * A060939  a(n) = (Sum of the first n primes) + n.
 * https://oeis.org/A060939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = (sum of the first n primes) + n
 * Range:     u8 n <= 13, u16 n <= 156, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

/* Local static state                                                   */
static idx_t  *prime_list  = NULL;     /* list of primes                */
static seq_t  *cum_sum     = NULL;     /* cumulative sums, cum_sum[i] = 2+3+...+prime[i]   */
static uint8_t *sieve      = NULL;     /* temporary sieve (0 = prime, 1 = composite)       */
static size_t  prime_cap   = 0;        /* allocated capacity of prime_list             */
static size_t  cum_cap     = 0;        /* allocated capacity of cum_sum                */
static size_t  sieve_cap   = 0;        /* allocated size of sieve (size = limit+1)       */
static idx_t   prime_count = 0;        /* number of primes currently stored            */
static idx_t   last_limit  = 0;        /* last upper bound that was sieved             */

/* -------------------------------------------------------------------- */
/* Estimate an upper bound for the nth prime (Rosser–Schoenfeld).         */
static idx_t estimate_limit(idx_t n)
{
    if (n < 6)
        return 15;                         /* small fixed bound               */
    double nd = (double)n;
    double estimate = nd * (log(nd) + log(log(nd))) + 3.0;
    return (idx_t)estimate;
}

/* -------------------------------------------------------------------- */
/* Build a list of at least n primes and their cumulative sums.         */
static void ensure_primes(idx_t n)
{
    if (prime_count >= n)
        return;                             /* already have enough primes */

    idx_t limit = estimate_limit(n);

    /* Ensure that the sieve buffer is large enough.                   */
    while (1) {
        /* Allocate / resize sieve. */
        if ((size_t)limit + 1 > sieve_cap) {
            uint8_t *new_sieve = (uint8_t *)calloc((size_t)limit + 1, 1);
            free(sieve);
            sieve     = new_sieve;
            sieve_cap = (size_t)limit + 1;
        } else {
            /* Re-use existing buffer – just clear it. */
            memset(sieve, 0, sieve_cap);
        }

        /* Estimate how many primes we will need. For a rough bound use
           limit / log(limit) + 10. If that is too small, we'll loop again. */
        size_t need_cap = (limit / (size_t)log((double)limit)) + 10;
        if (prime_cap < need_cap) {
            idx_t *tmp = (idx_t *)realloc(prime_list, need_cap * sizeof(idx_t));
            prime_list = tmp;
            prime_cap  = need_cap;
        }
        if (cum_cap < need_cap) {
            seq_t *tmp = (seq_t *)realloc(cum_sum, need_cap * sizeof(seq_t));
            cum_sum = tmp;
            cum_cap = need_cap;
        }

        /* Run the sieve up to 'limit'.                            */
        size_t count = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (!sieve[i]) {                     /* i is prime                */
                if (count >= prime_cap) {        /* grow arrays on overflow   */
                    prime_cap *= 2;
                    idx_t *tmp = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
                    prime_list = tmp;
                }
                if (count >= cum_cap) {
                    cum_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cum_sum, cum_cap * sizeof(seq_t));
                    cum_sum = tmp;
                }

                prime_list[count] = i;
                if (count == 0)
                    cum_sum[count] = (seq_t)i;
                else
                    cum_sum[count] = cum_sum[count - 1] + (seq_t)i;
                ++count;

                /* Mark multiples of i. */
                if ((uint64_t)i * (uint64_t)i <= (uint64_t)limit) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        sieve[j] = 1;
                }
            }
        }

        /* If we found enough primes, remember info and exit.  */
        if (count >= n) {
            prime_count = count;
            last_limit  = limit;
            return;
        }

        /* Otherwise, increase the limit and try again.          */
        limit *= 2;
    }
}

/* -------------------------------------------------------------------- */
/* Public function with external linkage.                                */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                            /* undefined below offset */

    ensure_primes(n);
    seq_t sum_primes = cum_sum[n - 1];       /* sum of first n primes   */
    return sum_primes + (seq_t)n;           /* add n to the sum       */
}
